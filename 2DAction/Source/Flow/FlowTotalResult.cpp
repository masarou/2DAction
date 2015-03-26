/* ====================================================================== */
/**
 * @brief  
 *		�Q�[�����ʉ�ʃN���X
 * @note
 *		�e�X�e�[�W�̃X�R�A��\�����āA���Z���o�����
 */
/* ====================================================================== */

#include <fstream>
#include <algorithm>
#include "FlowTotalResult.h"
#include "Game/GameRecorder.h"
#include "Common/Utility/CommonGameUtility.h"
#include "System/Draw2D/SystemDraw2DResource.h"

FlowBase *FlowTotalResult::Create( const std::string &fileName )
{
	return NEW FlowTotalResult(fileName);
}

FlowTotalResult::FlowTotalResult( const std::string &fileName )
: FlowBase(fileName)
, m_pResultTex(NULL)
{
	DEBUG_PRINT("FlowTotalResult�����I�I\n");
}


FlowTotalResult::~FlowTotalResult(void)
{
	DEBUG_PRINT("FlowTotalResult�폜�I�I\n");
}

bool FlowTotalResult::Init()
{
	// �w�i�ꖇ�G�쐬
	m_pResultTex = TotalResult2D::CreateTotalResult2D();
	return true;
}

bool FlowTotalResult::FinishFlow()
{
	// �풓���ŏ��������K�v�Ȃ��̃��Z�b�g
	Utility::GameInitALL();

	return true;
}

/* ================================================ */
/**
 * @brief	�p�b�h����֐�
 */
/* ================================================ */
void FlowTotalResult::PadEventDecide()
{
	if( m_pResultTex && m_pResultTex->ProceedNext() ){
		StartFade("title");

		// ����̃v���C�𔽉f�����������L���O�쐬
		Common::SAVE_SCORE scoreLog;
		bool retVal = Utility::GetSaveRanking( scoreLog );
		if( retVal ){
			UpdateSortRanking( scoreLog );
		}
	
		// �v���C���O�����o��
		FILE *fpWrite = fopen( "playLog.dat", "wb" );
		if( fpWrite == NULL ){
			return;
		}
		fwrite( &scoreLog, sizeof(scoreLog), 1, fpWrite );
		fclose( fpWrite );
	}
}

bool sortScore(const uint32_t &left, const uint32_t &rRight)
{
	return left > rRight;
}

void FlowTotalResult::UpdateSortRanking( Common::SAVE_SCORE &scoreData )
{
	// ���܂ł̃X�R�A��push
	std::vector<uint32_t> ranking;
	for( uint32_t i = 0; i < NUMBEROF(scoreData.m_scoreTimeAttack) ; ++i ){
		ranking.push_back(scoreData.m_scoreTimeAttack[i]);
	}

	// ����̃X�R�A��push
	uint32_t scoreCurr = GameRecorder::GetInstance()->GetTotalScore();
	ranking.push_back(scoreCurr);

	// ���ёւ�
	std::sort( ranking.begin(), ranking.end(), sortScore);
	
	for( uint32_t i = 0; i < NUMBEROF(scoreData.m_scoreTimeAttack) ; ++i ){
		scoreData.m_scoreTimeAttack[i] = ranking.at(i);
	}
}

/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		�\������X�R�A���̂���
 */
/* ====================================================================== */
TotalResult2D *TotalResult2D::CreateTotalResult2D()
{
	return NEW TotalResult2D();
}

TotalResult2D::TotalResult2D()
: TaskUnit("TotalResult2D")
, m_dispState(DISP_STAGE01)
{
	for( uint32_t i = 0; i < DISP_MAX ; ++i ){
		m_pNumCounter[i] = NULL;
	}
}

TotalResult2D::~TotalResult2D(void)
{
	m_textureBG.DeleteAndInit();
	m_textureResult.DeleteAndInit();
}

bool TotalResult2D::Init()
{
	// �w�i�Z�b�g
	m_textureBG.Init();
	m_textureBG.m_pTex2D = NEW Game2DBase("titleBg.json");
	m_textureBG.m_texInfo.m_fileName = "titleBg.json";
	m_textureBG.m_texInfo.m_posOrigin.x = WINDOW_WIDTH / 2.0f;
	m_textureBG.m_texInfo.m_posOrigin.y = WINDOW_HEIGHT / 2.0f;
	m_textureBG.m_texInfo.m_usePlayerOffset = false;
	m_textureBG.m_texInfo.m_prioity = PRIORITY_LOWEST;
	m_textureBG.m_pTex2D->SetDrawInfo(m_textureBG.m_texInfo);

	// �X�e�[�^�X���j���[�̃p�[�c���擾
	Utility::GetPartsInfoFromJson( "gameResult.json", m_partsMap );

	// ��ʃt���[���Z�b�g
	m_textureResult.Init();
	m_textureResult.m_pTex2D = NEW Game2DBase("gameResult.json");
	m_textureResult.m_texInfo.m_fileName = "gameResult.json";
	m_textureResult.m_texInfo.m_posOrigin.x = WINDOW_WIDTH / 2.0f;
	m_textureResult.m_texInfo.m_posOrigin.y = WINDOW_HEIGHT / 2.0f;
	m_textureResult.m_texInfo.m_usePlayerOffset = false;
	m_textureBG.m_texInfo.m_prioity = PRIORITY_LOW;
	m_textureResult.m_pTex2D->SetDrawInfo(m_textureResult.m_texInfo);

	// �����\���p�摜���
	m_numberInfo.Init();
	m_numberInfo.m_usePlayerOffset = false;

	// �����J�E���^�̏�����
	// �����\���p�摜���Z�b�g
	static const std::string s_partsStr[DISP_MAX] = {
		"strNumber01",
		"strNumber02",
		"strNumber03",
		"strNumber04",
	};
	for( uint32_t i = 0; i < DISP_MAX ; ++i ){
		m_pNumCounter[i] = NumberCounter::Create("numberLarge.json");
		m_numberInfo.m_posOrigin = GetPartsPos( s_partsStr[i] );
		m_pNumCounter[i]->SetDrawInfo( m_numberInfo );
	}

	// �G��|���ē������_���Z�b�g
	m_pNumCounter[static_cast<uint32_t>(DISP_STAGE01)]->AddValue( GameRecorder::GetInstance()->GetStageScore( GameRecorder::STATE_STAGE01 ) );

	return true;
}

void TotalResult2D::Update()
{
	CallPadEvent();

	// �A�j���J�E���g���I����Ă���Ȃ玟�̃X�e�b�v�ɐi��
	uint32_t index = static_cast<uint32_t>( m_dispState );
	switch(m_dispState){
	case DISP_STAGE01:
		if( !m_pNumCounter[index]->IsPlayCountAnim() ){
			m_dispState = DISP_STAGE02;
			m_pNumCounter[static_cast<uint32_t>(DISP_STAGE02)]->AddValue( GameRecorder::GetInstance()->GetStageScore( GameRecorder::STATE_STAGE02 ) );
		}
		break;
	case DISP_STAGE02:
		if( !m_pNumCounter[index]->IsPlayCountAnim() ){
			m_dispState = DISP_STAGE03;
			m_pNumCounter[static_cast<uint32_t>(DISP_STAGE03)]->AddValue( GameRecorder::GetInstance()->GetStageScore( GameRecorder::STATE_STAGE03 ) );
		}
		break;
	case DISP_STAGE03:
		if( !m_pNumCounter[index]->IsPlayCountAnim() ){
			m_dispState = DISP_TOTAL;
			m_pNumCounter[static_cast<uint32_t>(DISP_TOTAL)]->AddValue( GameRecorder::GetInstance()->GetTotalScore() );
		}
		break;
	case DISP_TOTAL:
		if( !m_pNumCounter[index]->IsPlayCountAnim() ){
			m_dispState = DISP_MAX;
		}
		break;
	case DISP_MAX:

		break;
	}

}

void TotalResult2D::DrawUpdate()
{
	// �w�i�`��
	if( m_textureBG.m_pTex2D ){
		m_textureBG.m_pTex2D->DrawUpdate2D();
	}
	if( m_textureResult.m_pTex2D ){
		m_textureResult.m_pTex2D->DrawUpdate2D();
	}
}

void TotalResult2D::PadEventDecide()
{
	uint32_t index = static_cast<uint32_t>( m_dispState );
	switch(m_dispState){
	case DISP_STAGE01:
		// �J�E���g�A�j���I��
		m_pNumCounter[index]->CountAnimEnd();
		m_dispState = DISP_STAGE02;

		m_pNumCounter[static_cast<uint32_t>(DISP_STAGE02)]->AddValue( GameRecorder::GetInstance()->GetStageScore( GameRecorder::STATE_STAGE02 ) );
		break;
	case DISP_STAGE02:
		// �J�E���g�A�j���I��
		m_pNumCounter[index]->CountAnimEnd();
		m_dispState = DISP_STAGE03;

		m_pNumCounter[static_cast<uint32_t>(DISP_STAGE03)]->AddValue( GameRecorder::GetInstance()->GetStageScore( GameRecorder::STATE_STAGE03 ) );
		break;
	case DISP_STAGE03:
		// �J�E���g�A�j���I��
		m_pNumCounter[index]->CountAnimEnd();
		m_dispState = DISP_TOTAL;

		m_pNumCounter[static_cast<uint32_t>(DISP_TOTAL)]->AddValue( GameRecorder::GetInstance()->GetTotalScore() );
		break;
	case DISP_TOTAL:
		// �J�E���g�A�j���I��
		m_pNumCounter[index]->CountAnimEnd();
		m_dispState = DISP_MAX;
		break;
	case DISP_MAX:

		break;
	}
}

const math::Vector2 TotalResult2D::GetPartsPos( const std::string name ) const
{
	// �X�e�[�^�X���j���[�̍�����W�擾
	const TEX_INIT_INFO &resultMenuInfo = TextureResourceManager::GetInstance()->GetLoadTextureInfo( m_textureResult.m_texInfo.m_fileName.c_str() );
	math::Vector2 retPos = m_textureResult.m_texInfo.m_posOrigin;
	retPos -= math::Vector2( resultMenuInfo.m_sizeWidth / 2.0f, resultMenuInfo.m_sizeHeight / 2.0f );

	// ��������p�[�c�̈ʒu�𑫂��Z
	Common::PARTS_INFO info = GetPartsInfo(name);
	retPos += info.m_pos;
	return retPos;
}

const Common::PARTS_INFO &TotalResult2D::GetPartsInfo( const std::string name ) const
{
	auto it = m_partsMap.find( name.c_str() );
	if( it != m_partsMap.end() ){
		return (*it).second;
	}
	DEBUG_ASSERT( 0, "�p�[�c��������܂���\n" );
	return (*m_partsMap.begin()).second;
}