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
#include "FlowStageResult.h"
#include "FlowManager.h"
#include "Game/GameRecorder.h"
#include "Common/Utility/CommonGameUtility.h"

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
 *
 * @note
 *		�^�C�g���ꖇ�G�N���X
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
}

bool Result2D::Init()
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

void Result2D::Update()
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

void Result2D::DrawUpdate()
{
	// �w�i�`��
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
