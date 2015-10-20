/* ====================================================================== */
/**
 * @brief  
 *		�t���[�̂��ׂĂ��Ǘ�����Manager
 * @note
 *		
 */
/* ====================================================================== */
#include "typeinfo.h"
#include "FlowManager.h"
#include "Flow/FlowTitle.h"
#include "Flow/FlowTable.h"
#include "Game/GameRecorder.h"
#include "System/SystemFadeManager.h"
#include "System/SystemBgManager.h"

FlowManager* FlowManager::m_pInstance = NULL;
static const char *JSON_START_FLOW_PATH		= "Data/Json/Flow/FlowTitle.json";

FlowManager::FlowManager()
	: TaskUnit( "FlowManager" )
	, m_pFlow(NULL)
	, m_step(FLOW_CHANGE_WAIT)
	, m_fadeNext(FLOW_MAX)
	, m_nextFilePath("")
	, m_currFlow( Common::FLOW_TITLE )
	, m_preFlow( Common::FLOW_MAX )
{
};

FlowManager::~FlowManager()
{
}

bool FlowManager::DieMain()
{
	if(m_pFlow){
		for(;;){
			if(m_pFlow->Finish()){
				SAFE_DELETE(m_pFlow);
				return true;
			}
		}
	}
	return false;
}

void FlowManager::Create()
{
	if( m_pInstance == NULL ){
		m_pInstance = NEW FlowManager;
	}
}

FlowManager *FlowManager::GetInstance()
{
	return m_pInstance;
}


bool FlowManager::Init()
{
	//! m_pFlow�̍쐬
	m_pFlow = FlowTitle::Create(JSON_START_FLOW_PATH);
	m_step = FLOW_INIT;
	return true;
};

//! ���j�b�g�̓o�^
bool FlowManager::AddUnit(TaskUnit *unit)
{
	if(m_pFlow){
		m_pFlow->AddChildTask(unit);
		return true;
	}
	return false;
}


//! ���݂̃t���[���擾
const char *FlowManager::GetCurrentFlow() const
{
	if( m_pFlow ){
		return typeid(m_pFlow).name();
	}
	DEBUG_ASSERT( 0, "�t���[���Ȃ�!!");
	return "";
}

//! �X�e�[�W�G�t�F�N�g�ǉ�
void FlowManager::SetupSpecialEffect( ProcessBase *pEffect )
{
	if( m_pFlow ){
		m_pFlow->PushStageEffect( pEffect );
	}
}

void FlowManager::Update()
{
	if( !m_pFlow ){
		DEBUG_ASSERT( 0, "m_pFlow �� NULL");
		return;
	}

	switch( m_step ){
	case FLOW_MAX:
	default:
		DEBUG_ASSERT( 0, "FlowBase�̏�Ԃ����蓾�Ȃ�");
		break;

	case FLOW_FADEIN_CHECK:
		m_step = FLOW_FADEIN_WAIT;
		m_fadeNext = FLOW_CHANGE_WAIT;
		FadeManager::GetInstance()->StartFadeIn();
		
		// �������}���邽�߂��̃X�e�[�g�ł�Update���s��
		m_pFlow->UpdateFlow();
		break;

	//! �Ó]���̃t���[�ύX�����ۂɂ����Ȃ�
	case FLOW_CHANGE_WAIT:
		if(m_pFlow->Finish()){
			SAFE_DELETE(m_pFlow);
			m_step = FLOW_INIT;
			m_pFlow = FlowTable::CreateFlow( m_nextFilePath.c_str() );

			// ���݂̃t���[���X�V
			m_preFlow = m_currFlow;
			m_currFlow = FlowTable::GetGameFlowKind( m_nextFilePath.c_str() );
			GameRecorder *pRecorder = GameRecorder::GetInstance();
			if( pRecorder ){
				switch( m_currFlow ){
				case Common::FLOW_STAGE01:
					pRecorder->SetGameStateOfProgress( GameRecorder::STATE_STAGE01 );
					break;
				case Common::FLOW_STAGE02:
					pRecorder->SetGameStateOfProgress( GameRecorder::STATE_STAGE02 );
					break;
				case Common::FLOW_STAGE03:
					pRecorder->SetGameStateOfProgress( GameRecorder::STATE_STAGE03 );
					break;
				case Common::FLOW_STAGE04:
					pRecorder->SetGameStateOfProgress( GameRecorder::STATE_STAGE04 );
					break;
				case Common::FLOW_STAGE05:
					pRecorder->SetGameStateOfProgress( GameRecorder::STATE_STAGE05 );
					break;
				case Common::FLOW_STAGE06:
					pRecorder->SetGameStateOfProgress( GameRecorder::STATE_STAGE06 );
					break;
				case Common::FLOW_STAGE07:
					pRecorder->SetGameStateOfProgress( GameRecorder::STATE_STAGE07 );
					break;
				case Common::FLOW_STAGE08:
					pRecorder->SetGameStateOfProgress( GameRecorder::STATE_STAGE08 );
					break;
				case Common::FLOW_STAGE09:
					pRecorder->SetGameStateOfProgress( GameRecorder::STATE_STAGE09 );
					break;
				case Common::FLOW_STAGE10:
					pRecorder->SetGameStateOfProgress( GameRecorder::STATE_STAGE10 );
					break;
				}
			}
			m_nextFilePath = "";
		}
		break;

	//! �t���[�ύX��A�t���[�̏������҂��X�e�b�v
	case FLOW_INIT:
		// BG�̏����̃`�F�b�N�ƃt���[�̏����`�F�b�N
		if( BgManager::GetInstance() && BgManager::GetInstance()->IsShowingBG() ){
			if( m_pFlow && m_pFlow->Init() ){
				m_step = FLOW_FADEOUT_CHECK;
			}
		}
		break;

	//! �t���[�ύX��A�t���[�̏������҂��X�e�b�v
	case FLOW_FADEOUT_CHECK:
		m_step = FLOW_FADEOUT_WAIT;
		m_fadeNext = FLOW_ACTION;
		FadeManager::GetInstance()->StartFadeOut();

		// �������}���邽�߂��̃X�e�[�g�ł�Update���s��
		m_pFlow->UpdateFlow();
		break;

	//! �ʏ폈��
	case FLOW_ACTION:
		m_pFlow->UpdateFlow();
		break;

	//! �t���[�̕ύX���s�����߂̈Ó]�҂��X�e�b�v
	case FLOW_FADEIN_WAIT:
		if( FadeManager::GetInstance()->GetCurrentState() == FadeManager::STATE_OVERWINDOW )
		{
			m_step = m_fadeNext;
			m_fadeNext = FLOW_MAX;
		}
		else{
			m_pFlow->UpdateFlow();
		}
		break;
	case FLOW_FADEOUT_WAIT:
		if( FadeManager::GetInstance()->GetCurrentState() == FadeManager::STATE_IDLE )
		{
			m_step = m_fadeNext;
			m_fadeNext = FLOW_MAX;
		}
		m_pFlow->UpdateFlow();
		break;
	}
}

/* ================================================ */
/**
 * @brief	���̉�ʂ�
 */
/* ================================================ */
bool FlowManager::ChangeFlow(const char* filePath){
	if(m_pFlow && m_step == FLOW_ACTION ){
		m_step = FLOW_FADEIN_CHECK;
		m_nextFilePath = filePath;
		return true;
	}
	return false;
}