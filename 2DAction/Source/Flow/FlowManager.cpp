/* ====================================================================== */
/**
 * @brief  
 *		フローのすべてを管理するManager
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


//! 現在のフロー名取得
const char *FlowManager::GetCurrentFlow() const
{
	if( m_pFlow ){
		return typeid(m_pFlow).name();
	}
	DEBUG_ASSERT( 0, "フローがない!!");
	return "";
}

//! ステージエフェクト追加
void FlowManager::SetupSpecialEffect( ProcessBase *pEffect )
{
	if( m_pFlow ){
		m_pFlow->PushStageEffect( pEffect );
	}
}

void FlowManager::Update()
{
	if( !m_pFlow ){
		DEBUG_ASSERT( 0, "m_pFlow が NULL");
		return;
	}

	switch( m_step ){
	case FLOW_MAX:
	default:
		DEBUG_ASSERT( 0, "FlowBaseの状態があり得ない");
		break;

	case FLOW_FADEIN_CHECK:
		m_step = FLOW_FADEIN_WAIT;
		m_fadeNext = FLOW_CHANGE_WAIT;
		FadeManager::GetInstance()->StartFadeIn();
		
		// ちらつきを抑えるためこのステートでもUpdateを行う
		m_pFlow->UpdateFlow();
		break;

	//! 暗転時のフロー変更を実際におこなう
	case FLOW_CHANGE_WAIT:
		if(m_pFlow->Finish()){
			SAFE_DELETE(m_pFlow);
			m_step = FLOW_INIT;
			m_pFlow = FlowTable::CreateFlow( m_nextFilePath.c_str() );

			// 現在のフローを更新
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

	//! フロー変更後、フローの初期化待ちステップ
	case FLOW_INIT:
		// BGの準備のチェックとフローの準備チェック
		if( BgManager::GetInstance() && BgManager::GetInstance()->IsShowingBG() ){
			if( m_pFlow && m_pFlow->Init() ){
				m_step = FLOW_FADEOUT_CHECK;
			}
		}
		break;

	//! フロー変更後、フローの初期化待ちステップ
	case FLOW_FADEOUT_CHECK:
		m_step = FLOW_FADEOUT_WAIT;
		m_fadeNext = FLOW_ACTION;
		FadeManager::GetInstance()->StartFadeOut();

		// ちらつきを抑えるためこのステートでもUpdateを行う
		m_pFlow->UpdateFlow();
		break;

	//! 通常処理
	case FLOW_ACTION:
		m_pFlow->UpdateFlow();
		break;

	//! フローの変更を行うための暗転待ちステップ
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
 * @brief	次の画面へ
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