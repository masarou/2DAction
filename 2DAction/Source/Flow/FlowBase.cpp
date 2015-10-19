/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		フローの共通項目、json読み取り等の機能を持たせたクラス
 */
/* ====================================================================== */
#include "System/picojson.h"
#include "FlowBase.h"
#include "FlowManager.h"
#include "Process/FlowProcessBase.h"
#include "Common/Utility/CommonGameUtility.h"
#include "System/SystemFadeManager.h"
#include "System/SystemBgManager.h"
#include "System/Message/SystemMessageManager.h"
#include "System/Collision/SystemCollisionManager.h"


FlowBase::FlowBase( const std::string &fileName)
: m_filePath(fileName)
, m_isInvalidPadCtrl( false )
{
	LoadFlowFile();
}

FlowBase::~FlowBase(void)
{
}

/* ================================================ */
/**
 * @brief	次の画面へ
 */
/* ================================================ */
bool FlowBase::StartFade(const char* eventStr)
{
	for(uint32_t i = 0; i < m_vEventName.size(); ++i){
		if(m_vEventName.at(i).eventStr.compare(eventStr) == 0){
			bool changeResult = FlowManager::GetInstance()->ChangeFlow(m_vEventName.at(i).filePath.c_str());
			if( changeResult ){
				m_isInvalidPadCtrl = true;
			}
			return changeResult;
		}
	}
	DEBUG_ASSERT( 0, "event名がない");
	return false;
}

/* ============================================== */
/**
 * @brief	終了処理
 */
/* ================================================ */
bool FlowBase::Finish()
{
	// 子タスクの終了処理
	if( !ReleaseAllEnd() ){
		return false;
	}

	// ステージエフェクト終了処理
	auto it = m_vStageEffect.begin();
	while( it != m_vStageEffect.end() ){
		if( (*it)->CanDie() ){
			ProcessBase *tmp = *it;
			it = m_vStageEffect.erase(it);
			SAFE_DELETE(tmp);
		}
		if( it != m_vStageEffect.end() ){
			++it;
		}
	}
	if( !m_vStageEffect.empty() ){
		return false;
	}

	//! 派生先の終了確認
	return FinishFlow();
}

/* ============================================== */
/**
 * @brief	jsonファイルの読み込み
 */
/* ================================================ */
void FlowBase::LoadFlowFile()
{
	idxfstream ifs(m_filePath.c_str());

	picojson::value root;
	picojson::parse( root, ifs);
	
	picojson::value null;
	picojson::value sceneData = root.get("eventdata");
	for(uint32_t i = 0;; ++i){
		if( sceneData == null || sceneData.get(i) == null){
			break;
		}
		FLOW_DATA data;
		data.eventStr = sceneData.get(i).get("event").get<std::string>();
		data.filePath = sceneData.get(i).get("path").get<std::string>();
		m_vEventName.push_back(data);
	}
	if(m_vEventName.empty()){
		DEBUG_ASSERT( 0, "m_vEventName is empty!!");
	}

	//BG変更処理
	picojson::value bgData = root.get("bgdata");
	if( BgManager::GetInstance() && bgData != null ){
		std::string bgStr = bgData.get(0).get("bgid").get<std::string>();
		BgManager::GetInstance()->SetNextBg( bgStr );
	}
}

/* ============================================== */
/**
 * @brief	子タスク更新処理
 */
/* ================================================ */
void FlowBase::UpdateFlow()
{
	// 派生先の子タスク更新前Update
	UpdateFlowPreChildTask();

	if( !m_isInvalidPadCtrl ){
		CallPadEvent();	// パッド入力取得
	}

	// 子の更新
	ChildUpdate();

	// 派生先の子タスク更新後Update
	UpdateFlowAfterChildTask();
	
	// 遷移先のチェック
	CheckNextFlow();
}


void FlowBase::PadEventStart()
{
	if( Utility::IsGamePause() ){
		Utility::EndGamePause();
	}
	else{
		Utility::StartGamePause();
	}
};

/* ============================================== */
/**
 * @brief	子タスク追加
 */
/* ================================================ */
void FlowBase::AddChildTask(TaskUnit *pTask)
{
	if(pTask){
		m_vTaskUnit.push_back(pTask);
	}
}

/* ============================================== */
/**
 * @brief	子タスク更新処理
 */
/* ================================================ */
void FlowBase::ChildUpdate()
{
	if( Utility::IsGamePause() ){
		// ポーズ中
	}
	else if( m_vStageEffect.size() != 0
		&& FadeManager::GetInstance()->GetCurrentState() == FadeManager::STATE_IDLE ){
		// フロー演出中
		auto it = m_vStageEffect.begin();
		if( (*it)->IsEffectEnd() ){
			ProcessBase *tmp = *it;
			m_vStageEffect.erase( it );
			SAFE_DELETE(tmp);

			if( m_vStageEffect.size() == 0 ){
				// フィルター終了
				Utility::EndGameStop();
			}
		}
		else{
			(*it)->Exec();
			// フィルターを表示し続ける
			Utility::StartGameStop( /*bool withFilter=*/(*it)->IsNeedPauseFilter() );
		}

		// Effectクラスが作られたときにInitだけは処理しないと描画されないのでここで処理
		for(uint32_t i = 0; i < m_vTaskUnit.size(); ++i){
			TaskUnit *pTask = m_vTaskUnit.at(i);
			if( pTask->GetStatus() == TaskUnit::TASK_INIT ){
				if(pTask->Init()){
					pTask->SetStatus(TaskUnit::TASK_ALIVE);
				}
			}
		}
	}
	else{
		Exec();				//! 位置等の更新
		CollisionManager::GetInstance()->CollisionUpdate();			// 衝突判定更新+各クラスにイベント発行	
		SystemMessageManager::GetInstance()->StartMessageEvent();	// 各クラスの相互イベント処理を行う
	}
	DrawUpdate();		//! 描画等の更新
}