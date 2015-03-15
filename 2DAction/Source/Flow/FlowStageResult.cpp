/* ====================================================================== */
/**
 * @brief  
 *		�Q�[���X�e�[�W���ʉ�ʃN���X
 * @note
 *		
 */
/* ====================================================================== */

#include <fstream>
#include <algorithm>
#include "FlowStageResult.h"
#include "FlowManager.h"
#include "Game/GameRecorder.h"
#include "Common/Utility/CommonGameUtility.h"
#include "System/Draw2D/SystemDraw2DResource.h"

FlowBase *FlowStageResult::Create( const std::string &fileName )
{
	return NEW FlowStageResult(fileName);
}

FlowStageResult::FlowStageResult( const std::string &fileName )
: FlowBase(fileName)
, m_pResultTex(NULL)
{
	DEBUG_PRINT("FlowStageResult�����I�I\n");
}


FlowStageResult::~FlowStageResult(void)
{
	DEBUG_PRINT("FlowStageResult�폜�I�I\n");
}

bool FlowStageResult::Init()
{
	// �w�i�ꖇ�G�쐬
	m_pResultTex = Result2D::CreateResult2D();
	return true;
}

/* ================================================ */
/**
 * @brief	�p�b�h����֐�
 */
/* ================================================ */
void FlowStageResult::PadEventDecide()
{
	if( m_pResultTex && m_pResultTex->ProceedNext() ){
		// ���ɐi��
		GameRecorder *pRecorder = GameRecorder::GetInstance();
		if( !pRecorder ){
			DEBUG_ASSERT( 0, "GameRecorder is NULL");
			return;
		}
		switch( pRecorder->GetGameStateOfProgress() ){
		case GameRecorder::STATE_TITLE:
		default:
			DEBUG_ASSERT( 0, "�z��O�̃t���[" );
			// �Ƃ肠�����^�C�g����
			StartFade( "title" );
			break;
		case GameRecorder::STATE_STAGE01:
		case GameRecorder::STATE_STAGE02:
			StartFade( "interval" );
			break;
		case GameRecorder::STATE_STAGE03:
			StartFade( "totalresult" );	// ���ׂẴX�e�[�W�I��
			break;
		}	
		// ����SE�炷
		SoundManager::GetInstance()->PlaySE("Decide");
	}
}

/* ====================================================================== */
/**
 * @brief  
 *			�X�e�[�W���ʂ̒��g�J�E���^�N���X
 * @note
 *		
 */
/* ====================================================================== */
Result2D *Result2D::CreateResult2D()
{
	return NEW Result2D();
}

Result2D::Result2D()
: TaskUnit("Result2D")
, m_dispState(DISP_RESULT)
, m_pNumCounterResult( NULL )
, m_pNumCounterBonus( NULL )
, m_pNumCounterTotal( NULL )
{
}

Result2D::~Result2D(void)
{
	m_textureResult.DeleteAndInit();
	m_textureBG.DeleteAndInit();
}

bool Result2D::Init()
{
	// �w�i�Z�b�g
	m_textureBG.Init();
	m_textureBG.m_pTex2D = NEW Game2DBase("title.json");
	m_textureBG.m_texInfo.Init();
	m_textureBG.m_texInfo.m_fileName = "title.json";
	m_textureBG.m_texInfo.m_posOrigin.x = WINDOW_WIDTH / 2.0f;
	m_textureBG.m_texInfo.m_posOrigin.y = WINDOW_HEIGHT / 2.0f;
	m_textureBG.m_texInfo.m_usePlayerOffset = false;
	m_textureBG.m_texInfo.m_prioity = PRIORITY_LOWEST;
	m_textureBG.m_pTex2D->SetDrawInfo(m_textureBG.m_texInfo);

	// �X�e�[�^�X���j���[�̃p�[�c���擾
	Utility::GetPartsInfoFromJson( "stageResult.json", m_partsMap );

	// ��ʃt���[���Z�b�g
	m_textureResult.Init();
	m_textureResult.m_pTex2D = NEW Game2DBase("stageResult.json");
	m_textureResult.m_texInfo.Init();
	m_textureResult.m_texInfo.m_fileName = "stageResult.json";
	m_textureResult.m_texInfo.m_posOrigin.x = WINDOW_WIDTH / 2.0f;
	m_textureResult.m_texInfo.m_posOrigin.y = WINDOW_HEIGHT / 2.0f;
	m_textureResult.m_texInfo.m_usePlayerOffset = false;
	m_textureBG.m_texInfo.m_prioity = PRIORITY_LOW;
	m_textureResult.m_pTex2D->SetDrawInfo(m_textureResult.m_texInfo);

	// �����J�E���^�̏�����
	m_pNumCounterResult = NumberCounter::Create("numberLarge.json");
	m_pNumCounterBonus = NumberCounter::Create("numberLarge.json");
	m_pNumCounterTotal = NumberCounter::Create("numberLarge.json");

	// �����\���p�摜���
	m_numberInfo.Init();
	m_numberInfo.m_usePlayerOffset = false;

	// �����\���p�摜���Z�b�g
	m_numberInfo.m_posOrigin = GetPartsPos( "strNumber01" );
	m_pNumCounterResult->SetDrawInfo( m_numberInfo );
	m_numberInfo.m_posOrigin = GetPartsPos( "strNumber02" );
	m_pNumCounterBonus->SetDrawInfo( m_numberInfo );
	m_numberInfo.m_posOrigin = GetPartsPos( "strNumber04" );
	m_pNumCounterTotal->SetDrawInfo( m_numberInfo );

	// �G��|���ē������_���Z�b�g
	m_pNumCounterResult->AddValue( GameRecorder::GetInstance()->GetScore() );

	return true;
}

void Result2D::Update()
{
	CallPadEvent();

	// �A�j���J�E���g���I����Ă���Ȃ玟�̃X�e�b�v�ɐi��
	switch(m_dispState){
	case DISP_RESULT:
		if( !m_pNumCounterResult->IsPlayCountAnim() ){
			m_dispState = DISP_BONUS;
			m_pNumCounterBonus->AddValue( GetStageClearBonus() );		
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

void Result2D::DrawUpdate()
{
	// �w�i�`��
	if( m_textureBG.m_pTex2D ){
		m_textureBG.m_pTex2D->DrawUpdate2D();
	}
	if( m_textureResult.m_pTex2D ){
		m_textureResult.m_pTex2D->DrawUpdate2D();
	}
}

void Result2D::PadEventDecide()
{
	switch(m_dispState){
	case DISP_RESULT:
		// �J�E���g�A�j���I��
		m_pNumCounterResult->CountAnimEnd();
		m_dispState = DISP_BONUS;

		m_pNumCounterBonus->AddValue( GetStageClearBonus() );
		break;
	case DISP_BONUS:
		// �J�E���g�A�j���I��
		m_pNumCounterBonus->CountAnimEnd();
		m_dispState = DISP_TOTAL;

		m_pNumCounterTotal->AddValue( m_pNumCounterResult->GetValue() + m_pNumCounterBonus->GetValue() );
		break;
	case DISP_TOTAL:
		// �J�E���g�A�j���I��
		m_pNumCounterTotal->CountAnimEnd();
		m_dispState = DISP_ALL;
		
		// �����ŋ��߂��{�[�i�X���X�e�[�W�X�R�A�ɉ��Z
		GameRecorder::GetInstance()->AddScoreBonus( m_pNumCounterBonus->GetValue() );
		break;
	case DISP_ALL:

		break;
	}
}

uint32_t Result2D::GetStageClearBonus() const
{
	// �{�[�i�X�Z�o���@
	// �R���{���~���[�U�[�̎c�胉�C�t = Bonus

	uint32_t retVal = 0;
	uint32_t stageComboMax = GameRecorder::GetInstance()->GetMaxComboNumOfStage();

	enum SCORE_STAGE {
		SCORE_01 = 100,
		SCORE_02 = 200,
		SCORE_03 = 300,
		SCORE_04 = 400,
		SCORE_05 = 500,
	};
	SCORE_STAGE scoreStage = SCORE_01;
	for( uint32_t i = 0; i < stageComboMax ; ++i ){
		switch( scoreStage ){
		case SCORE_01:
			retVal += SCORE_01;
			if( i > 20 ){
				scoreStage = SCORE_02;
			}
			break;
		case SCORE_02:
			retVal += SCORE_02;
			if( i > 50 ){
				scoreStage = SCORE_03;
			}
			break;
		case SCORE_03:
			retVal += SCORE_03;
			if( i > 80 ){
				scoreStage = SCORE_04;
			}
			break;
		case SCORE_04:
			retVal += SCORE_04;
			if( i > 100 ){
				scoreStage = SCORE_05;
			}
			break;
		case SCORE_05:
			retVal += SCORE_05;
			break;
		}
	}
	// �c�X�R�A��������
	retVal = static_cast<uint32_t>( retVal*GameRecorder::GetInstance()->GetUserLifeRatio() );
	return retVal;
}

const math::Vector2 Result2D::GetPartsPos( const std::string name ) const
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

const Common::PARTS_INFO &Result2D::GetPartsInfo( const std::string name ) const
{
	auto it = m_partsMap.find( name.c_str() );
	if( it != m_partsMap.end() ){
		return (*it).second;
	}
	DEBUG_ASSERT( 0, "�p�[�c��������܂���\n" );
	return (*m_partsMap.begin()).second;
}