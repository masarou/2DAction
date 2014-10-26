
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

FlowBase::FlowBase(std::string fileName)
	: m_fileName(fileName)
{
	LoadFlowFile();
}

FlowBase::~FlowBase(void)
{
}

/* ============================================== */
/**
 * @brief	終了処理
 */
/* ================================================ */
bool FlowBase::Finish()
{
	if(ReleaseAllEnd()){
		return true;
	}
	//! まだ子タスクの終了待ち
	return false;
}

/* ============================================== */
/**
 * @brief	子タスク更新処理
 */
/* ================================================ */
void FlowBase::ChildUpdate()
{
	Update();			//! 位置等の更新
	CollisionUpdate();	//! 衝突判定更新+各クラスにイベント発行
	EventUpdate();		//! 各クラスのイベント処理
	DrawUpdate();		//! 描画等の更新
}

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
 * @brief	jsonファイルの読み込み
 */
/* ================================================ */
void FlowBase::LoadFlowFile()
{
	std::ifstream ifs(m_fileName.c_str());

	picojson::value root;
	picojson::parse( root, ifs);
	
	picojson::value sceneData = root.get("eventdata");
	for(uint32_t i = 0;; ++i){
		picojson::value null;
		if(sceneData.get(i) == null){
			break;
		}
		FLOW_DATA data;
		data.eventStr = sceneData.get(i).get("event").get<std::string>();
		data.filePath = sceneData.get(i).get("path").get<std::string>();
		m_vEventName.push_back(data);
	}

	if(m_vEventName.empty()){
		DEBUG_ASSERT("m_vEventName is empty!!");
	}

}

/* ================================================ */
/**
 * @brief	次の画面へ
 */
/* ================================================ */
void FlowBase::StartFade(const char* eventStr)
{
	for(uint32_t i = 0; i < m_vEventName.size(); ++i){
		if(m_vEventName.at(i).eventStr.compare(eventStr) == 0){
			FlowManager::GetInstance()->ChangeFlow(m_vEventName.at(i).filePath.c_str());
			return;
		}
	}
	DEBUG_ASSERT("event名がない");
}
