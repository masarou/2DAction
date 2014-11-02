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
	if( !m_pFlow ){
		DEBUG_ASSERT("m_pFlow が NULL");
		return;
	}

	switch( m_step ){
	case FLOW_MAX:
	default:
		DEBUG_ASSERT("FlowBaseの状態があり得ない");
		break;

	case FLOW_FADEIN_CHECK:
		m_step = FLOW_FADEIN_WAIT;
		m_fadeNext = FLOW_CHANGE_WAIT;
		FadeManager::GetInstance()->StartFadeIn();
		
		// ちらつきを抑えるためこのステートでもFlowUpdateを行う
		m_pFlow->FlowUpdate();
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
		if( m_pFlow && m_pFlow->Init() ){
			m_step = FLOW_FADEOUT_CHECK;
		}
		break;

	//! フロー変更後、フローの初期化待ちステップ
	case FLOW_FADEOUT_CHECK:
		m_step = FLOW_FADEOUT_WAIT;
		m_fadeNext = FLOW_ACTION;
		FadeManager::GetInstance()->StartFadeOut();

		// ちらつきを抑えるためこのステートでもFlowUpdateを行う
		m_pFlow->FlowUpdate();
		break;

	//! 通常処理
	case FLOW_ACTION:
		m_pFlow->FlowUpdate();
		break;

	//! フローの変更を行うための暗転待ちステップ
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
 * @brief	次の画面へ
 */
/* ================================================ */
void FlowManager::ChangeFlow(const char* filePath){
	if(m_pFlow && m_step == FLOW_ACTION ){
		m_step = FLOW_FADEIN_CHECK;
		m_nextFilePath = filePath;
	}
}