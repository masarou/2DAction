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

FlowManager* FlowManager::m_pInstance = NULL;
static const char *JSON_START_FLOW_PATH		= "Data/Json/Flow/FlowTitle.json";

FlowManager::FlowManager()
	: TaskUnit( "FlowManager" )
	, m_pFlow(NULL)
	, m_step(FLOW_IDLE)
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
	switch(m_step){
	case FLOW_MAX:
	default:
		DEBUG_ASSERT("FlowBase�̏�Ԃ����蓾�Ȃ�");
		break;

	case FLOW_IDLE:
		m_step = FLOW_FADEIN_WAIT;
		m_fadeNext = FLOW_INIT;
		break;

	//! �ʏ폈��
	case FLOW_ACTION:
		if(m_pFlow){
			m_pFlow->FlowUpdate();
		}
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
		if(m_pFlow && m_pFlow->Init()){
			m_step = FLOW_FADEOUT_WAIT;
			m_fadeNext = FLOW_ACTION;
			//sys::SystemFade::StartFadeOut();
		}
		break;

	//! �t���[�̕ύX���s�����߂̈Ó]�҂��X�e�b�v
	case FLOW_FADEIN_WAIT:
	case FLOW_FADEOUT_WAIT:
		//if(!sys::SystemFade::IsBusy()){
			m_step = m_fadeNext;
			m_fadeNext = FLOW_MAX;
		//}
		break;
	}
}

/* ================================================ */
/**
 * @brief	���̉�ʂ�
 */
/* ================================================ */
void FlowManager::ChangeFlow(const char* filePath){
	if(m_pFlow /*&& sys::SystemFade::StartFadeIn()*/){
		m_step = FLOW_FADEIN_WAIT;
		m_fadeNext = FLOW_CHANGE_WAIT;
		m_nextFilePath = filePath;
	}
	else{
		DEBUG_ASSERT("m_pFlow�����ł�NULL��StartFadeIn���s");
	}
}