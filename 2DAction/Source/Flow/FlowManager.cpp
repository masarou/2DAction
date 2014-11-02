/* ====================================================================== */
/**
 * @brief  �t���[�̂��ׂĂ��Ǘ������ԑ傫���N���X
 *
 * @note �g�p���@
	
 */
/* ====================================================================== */

#include "FlowManager.h"
#include "Flow/FlowTitle.h"
#include "Flow/FlowTable.h"
#include "System/SystemFadeManager.h"

FlowManager* FlowManager::m_pInstance = NULL;
static const char *JSON_START_FLOW_PATH		= "Data/Json/Flow/FlowTitle.json";

FlowManager::FlowManager()
	: TaskUnit( "FlowManager" )
	, m_pFlow(NULL)
	, m_step(FLOW_CHANGE_WAIT)
	, m_fadeNext(FLOW_MAX)
	, m_nextFilePath("")
{
};

FlowManager::~FlowManager()
{
}

bool FlowManager::DieMain()
{
	if(m_pFlow){
		for(;;){
			if(m_pFlow->ReleaseAllEnd()){
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

void FlowManager::Update()
{

}

void FlowManager::DrawUpdate()
{
	if( !m_pFlow ){
		DEBUG_ASSERT("m_pFlow �� NULL");
		return;
	}

	switch( m_step ){
	case FLOW_MAX:
	default:
		DEBUG_ASSERT("FlowBase�̏�Ԃ����蓾�Ȃ�");
		break;

	case FLOW_FADEIN_CHECK:
		m_step = FLOW_FADEIN_WAIT;
		m_fadeNext = FLOW_CHANGE_WAIT;
		FadeManager::GetInstance()->StartFadeIn();
		
		// �������}���邽�߂��̃X�e�[�g�ł�FlowUpdate���s��
		m_pFlow->FlowUpdate();
		break;

	//! �Ó]���̃t���[�ύX�����ۂɂ����Ȃ�
	case FLOW_CHANGE_WAIT:
		if(m_pFlow->Finish()){
			SAFE_DELETE(m_pFlow);
			m_step = FLOW_INIT;
			m_pFlow = FlowTable::CreateFlow(m_nextFilePath.c_str());
			m_nextFilePath = "";
		}
		break;

	//! �t���[�ύX��A�t���[�̏������҂��X�e�b�v
	case FLOW_INIT:
		if( m_pFlow && m_pFlow->Init() ){
			m_step = FLOW_FADEOUT_CHECK;
		}
		break;

	//! �t���[�ύX��A�t���[�̏������҂��X�e�b�v
	case FLOW_FADEOUT_CHECK:
		m_step = FLOW_FADEOUT_WAIT;
		m_fadeNext = FLOW_ACTION;
		FadeManager::GetInstance()->StartFadeOut();

		// �������}���邽�߂��̃X�e�[�g�ł�FlowUpdate���s��
		m_pFlow->FlowUpdate();
		break;

	//! �ʏ폈��
	case FLOW_ACTION:
		m_pFlow->FlowUpdate();
		break;

	//! �t���[�̕ύX���s�����߂̈Ó]�҂��X�e�b�v
	case FLOW_FADEIN_WAIT:
		if( FadeManager::GetInstance()->GetCurrentState() == FadeManager::STATE_OVERWINDOW )
		{
			m_step = m_fadeNext;
			m_fadeNext = FLOW_MAX;
		}
		else{
			m_pFlow->FlowUpdate();
		}
		break;
	case FLOW_FADEOUT_WAIT:
		if( FadeManager::GetInstance()->GetCurrentState() == FadeManager::STATE_IDLE )
		{
			m_step = m_fadeNext;
			m_fadeNext = FLOW_MAX;
		}
		m_pFlow->FlowUpdate();
		break;
	}
}

/* ================================================ */
/**
 * @brief	���̉�ʂ�
 */
/* ================================================ */
void FlowManager::ChangeFlow(const char* filePath){
	if(m_pFlow && m_step == FLOW_ACTION ){
		m_step = FLOW_FADEIN_CHECK;
		m_nextFilePath = filePath;
	}
}