/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		�Q�[�����ʉ�ʃN���X
 */
/* ====================================================================== */

#include <fstream>
#include <algorithm>
#include "FlowTotalResult.h"
#include "Game/GameRecorder.h"
#include "Common/Utility/CommonGameUtility.h"

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
	uint32_t scoreCurr = GameRecorder::GetInstance()->GetScore();
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
 *		�^�C�g���ꖇ�G�N���X
 */
/* ====================================================================== */
TotalResult2D *TotalResult2D::CreateTotalResult2D()
{
	return NEW TotalResult2D();
}

TotalResult2D::TotalResult2D()
: TaskUnit("TotalResult2D")
, m_dispState(DISP_RESULT)
, m_pNumCounterResult( NULL )
, m_pNumCounterBonus( NULL )
, m_pNumCounterTotal( NULL )
{
}

TotalResult2D::~TotalResult2D(void)
{
	m_textureResult.DeleteAndInit();
}

bool TotalResult2D::Init()
{
	// �w�i�Z�b�g
	m_textureResult.Init();
	m_textureResult.m_pTex2D = NEW Game2DBase("title.json");
	m_textureResult.m_texInfo.Init();
	m_textureResult.m_texInfo.m_posOrigin.x = WINDOW_WIDTH / 2.0f;
	m_textureResult.m_texInfo.m_posOrigin.y = WINDOW_HEIGHT / 2.0f;
	m_textureResult.m_texInfo.m_usePlayerOffset = false;
	m_textureResult.m_pTex2D->SetDrawInfo(m_textureResult.m_texInfo);

	// �����J�E���^�̏�����
	m_pNumCounterResult = NumberCounter::Create("number.json");
	m_pNumCounterBonus = NumberCounter::Create("number.json");
	m_pNumCounterTotal = NumberCounter::Create("number.json");

	// �����\���p�摜���
	m_numberInfo.Init();
	m_numberInfo.m_posOrigin.x = WINDOW_WIDTH / 2.0f + 500.0f;
	m_numberInfo.m_posOrigin.y = 150.0f;
	m_numberInfo.m_scale = math::Vector2(2.0f,2.0f);
	m_numberInfo.m_usePlayerOffset = false;

	// �����\���p�摜���Z�b�g
	m_pNumCounterResult->SetDrawInfo( m_numberInfo );
	m_numberInfo.m_posOrigin.y += 120.0f;
	m_pNumCounterBonus->SetDrawInfo( m_numberInfo );
	m_numberInfo.m_posOrigin.y += 200.0f;
	m_pNumCounterTotal->SetDrawInfo( m_numberInfo );

	// �G��|���ē������_���Z�b�g
	m_pNumCounterResult->AddValue( GameRecorder::GetInstance()->GetScore() );

	return true;
}

void TotalResult2D::Update()
{
	CallPadEvent();

	// �A�j���J�E���g���I����Ă���Ȃ玟�̃X�e�b�v�ɐi��
	switch(m_dispState){
	case DISP_RESULT:
		if( !m_pNumCounterResult->IsPlayCountAnim() ){
			m_dispState = DISP_BONUS;
			m_pNumCounterBonus->AddValue( GameRecorder::GetInstance()->GetScore() );
		}
		break;
	case DISP_BONUS:
		if( !m_pNumCounterBonus->IsPlayCountAnim() ){
			m_dispState = DISP_TOTAL;
			m_pNumCounterTotal->AddValue( m_pNumCounterResult->GetValue() + m_pNumCounterBonus->GetValue() );
		}
		break;
	case DISP_TOTAL:
		if( !m_pNumCounterTotal->IsPlayCountAnim() ){
			m_dispState = DISP_ALL;
		}
		break;
	case DISP_ALL:

		break;
	}

}

void TotalResult2D::DrawUpdate()
{
	// �w�i�`��
	if( m_textureResult.m_pTex2D ){
		m_textureResult.m_pTex2D->DrawUpdate2D();
	}
}

void TotalResult2D::PadEventDecide()
{

	switch(m_dispState){
	case DISP_RESULT:
		// �J�E���g�A�j���I��
		m_pNumCounterResult->CountAnimEnd();
		m_dispState = DISP_BONUS;

		m_pNumCounterBonus->AddValue( GameRecorder::GetInstance()->GetScore() );
		break;
	case DISP_BONUS:
		// �J�E���g�A�j���I��
		m_pNumCounterBonus->CountAnimEnd();
		m_dispState = DISP_TOTAL;

		m_pNumCounterTotal->AddValue( GameRecorder::GetInstance()->GetScore() );
		break;
	case DISP_TOTAL:
		// �J�E���g�A�j���I��
		m_pNumCounterTotal->CountAnimEnd();
		m_dispState = DISP_ALL;
		break;
	case DISP_ALL:

		break;
	}
}
