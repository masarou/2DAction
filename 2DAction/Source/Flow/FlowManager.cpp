/* ====================================================================== */
/**
 * @brief  フローのすべてを管理する一番大きいクラス
 *
 * @note 使用方法
	
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
	//! m_pFlowの作成
	m_pFlow = FlowTitle::Create(JSON_START_FLOW_PATH);
	m_step = FLOW_INIT;
	return true;
};

//! ユニットの登録
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
		DEBUG_ASSERT("FlowBaseの状態があり得ない");
		break;

	case FLOW_IDLE:
		m_step = FLOW_FADEIN_WAIT;
		m_fadeNext = FLOW_INIT;
		break;

	//! 通常処理
	case FLOW_ACTION:
		if(m_pFlow){
			m_pFlow->FlowUpdate();
		}
		break;

	//! 暗転時のフロー変更を実際におこなう
	case FLOW_CHANGE_WAIT:
		if(m_pFlow->Finish()){
			SAFE_DELETE(m_pFlow);
			m_step = FLOW_INIT;
			m_pFlow = FlowTable::CreateFlow(m_nextFilePath.c_str());
			m_nextFilePath = "";
		}
		break;

	//! フロー変更後、フローの初期化待ちステップ
	case FLOW_INIT:
		if(m_pFlow && m_pFlow->Init()){
			m_step = FLOW_FADEOUT_WAIT;
			m_fadeNext = FLOW_ACTION;
			//sys::SystemFade::StartFadeOut();
		}
		break;

	//! フローの変更を行うための暗転待ちステップ
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
 * @brief	次の画面へ
 */
/* ================================================ */
void FlowManager::ChangeFlow(const char* filePath){
	if(m_pFlow /*&& sys::SystemFade::StartFadeIn()*/){
		m_step = FLOW_FADEIN_WAIT;
		m_fadeNext = FLOW_CHANGE_WAIT;
		m_nextFilePath = filePath;
	}
	else{
		DEBUG_ASSERT("m_pFlowがすでにNULLかStartFadeIn失敗");
	}
}