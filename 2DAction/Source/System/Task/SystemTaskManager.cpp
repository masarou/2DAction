
/* ====================================================================== */
/**
 * @brief  クラス間のやり取りをするクラス
 *
 * @note 使用方法
	新しく登録する場合はTaskUnitBaseを継承している場合は
	勝手にコンストラクタで登録して、デストラクタが呼ばれると
	削除されるので特に意識せず使用可能。
	登録すると毎フレームUpdateとDrawUpdateが呼ばれる。

 */
/* ====================================================================== */

#include "SystemTaskManager.h"
#include "SystemTaskUnit.h"
#include "System/Draw2D/SystemDraw2DManager.h"

TaskManager *TaskManager::m_pInstance = NEW TaskManager();

TaskManagerBase::TaskManagerBase(void)
{
	m_vTaskUnit.clear();
}

TaskManagerBase::~TaskManagerBase(void)
{
	std::vector<TaskUnit*>::iterator it = m_vTaskUnit.begin();
	while(m_vTaskUnit.empty() == 0){
		//! 基本的にすべてのタスクは自分で削除するのでここで m_vTaskUnit.size()は0のはず
		TaskUnit *task = *it;
		DEBUG_PRINT("/_/_/解放し忘れタスクがあるよ : %s/_/_/\n",task->GetName().c_str());
		it = m_vTaskUnit.erase(it);
		SAFE_DELETE(task);
	}
	m_vTaskUnit.clear();
}

/* ================================================ */
/**
 * @brief	タスク追加
 */
/* ================================================ */
void TaskManagerBase::AddUnit( TaskUnit *unit )
{
	if( !unit ){
		DEBUG_ASSERT( 0, "taskがNULL");
	}
	m_vTaskUnit.push_back(unit);
}

/* ================================================ */
/**
 * @brief	タスク更新処理
 */
/* ================================================ */
void TaskManagerBase::Exec()
{
	for(uint32_t i = 0; i < m_vTaskUnit.size(); ++i){
		TaskUnit *pTask = m_vTaskUnit.at(i);
		switch(pTask->GetStatus()){
		default:
			DEBUG_ASSERT( 0, "/_/_/task status が想定外/_/_/");
			break;

		case TaskUnit::TASK_IDLE:
			//! 何もしない
			break;

		case TaskUnit::TASK_INIT:
			if(pTask->Init()){
				pTask->SetStatus(TaskUnit::TASK_ALIVE);
			}
			break;

		case TaskUnit::TASK_ALIVE:
			pTask->Update();
			break;

		case TaskUnit::TASK_PRE_DIE:
			if(pTask->DieMain()){
				pTask->SetStatus(TaskUnit::TASK_DIE);
			}
			break;

		case TaskUnit::TASK_DIE:
			DEBUG_ASSERT( 0, "/_/_/ここに来るのはおかしい/_/_/");
			break;
		}
	}

	//! 状態がTASK_PRE_DIEになったタスクを解放する
	DeleteDieUnit();
}

void TaskManagerBase::DrawUpdate()
{
	for(uint32_t i = 0; i < m_vTaskUnit.size(); ++i){
		TaskUnit *pTask = m_vTaskUnit.at(i);
		if(pTask->GetStatus() == TaskUnit::TASK_ALIVE){
			pTask->DrawUpdate();
		}
	}
}

/* ================================================ */
/**
 * @brief	死亡タスク解放
 */
/* ================================================ */
void TaskManagerBase::DeleteDieUnit()
{
	std::vector<TaskUnit*>::iterator it = m_vTaskUnit.begin();
	while(it != m_vTaskUnit.end()){
		TaskUnit *task = *it;
		if(task->GetStatus() == TaskUnit::TASK_DIE){
			it = m_vTaskUnit.erase(it);
			SAFE_DELETE(task);
			continue;
		}
		if(it != m_vTaskUnit.end()){
			++it;
		}
	}
}

/* ================================================ */
/**
 * @brief	終了時のタスク解放待ち関数
 */
/* ================================================ */
bool TaskManagerBase::ReleaseAllEnd()
{
	for(unsigned int i = 0; i < m_vTaskUnit.size(); ++i){
		TaskUnit *pTask = m_vTaskUnit.at(i);
		if(pTask->DieMain()){
			pTask->SetStatus(TaskUnit::TASK_DIE);
		}
	}
	//! 状態がTASK_PRE_DIEになったタスクを解放する
	DeleteDieUnit();

	if(m_vTaskUnit.size() == 0){
		return true;
	}
	return false;
}
