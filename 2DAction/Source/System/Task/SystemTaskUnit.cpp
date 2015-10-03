
/* ====================================================================== */
/**
 * @brief  各クラスの最小単位のクラス
 *
 * @note
 *		基本的にはこのクラスを派生させたものを使用する。
 *		m_statusによって自動的にupdateが呼ばれたりdeleteされたりする。
 *		setStatusでUNIT_DEADにしてやると自動的に削除される。
 *		関数説明
 *			Init()		: 最初に必ず呼ばれる、trueを返すまで呼ばれ続ける
 *			Update()	: 毎フレーム呼ばれる、更新処理等に
 *			DrawUpdate(): 毎フレーム呼ばれる、描画等に
 *			DieMain()	: m_statusをUNIT_DEADにしたら呼ばれる。
 *						  trueを返すまで呼ばれ続け、その後クラスが解放される
 */
/* ====================================================================== */

#include "SystemTaskUnit.h"
#include "SystemTaskManager.h"
#include "Flow/FlowManager.h"

TaskUnit::TaskUnit(std::string name, TASK_STATUS status)
: m_status(status)
, m_name(name)
, m_pParent(NULL)
{
	bool isSet = false;

	//! フロー側にセットを試みる
	FlowManager *pFlow = FlowManager::GetInstance();
	if(pFlow){
		isSet = FlowManager::GetInstance()->AddUnit(this);
	}

	//! セットできなかったら汎用へセット
	if(!isSet){
		TaskManager::GetInstance()->AddUnit(this);
	}
}

const TaskUnit::TASK_STATUS &TaskUnit::GetStatus() const
{
	return m_status;
}

bool TaskUnit::IsDie()
{
	if( GetStatus() < TASK_PRE_DIE ){
		return true;
	}
	return false;
}

TaskUnit::~TaskUnit(void)
{
	//! 親クラスに自分が死ぬことを伝える
	if(m_pParent){
		m_pParent->ReleaseConnection(this);
	}
	for(uint32_t i = 0; i < m_pChildVec.size(); ++i){
		//! 親が死ぬから子も殺す
		m_pChildVec.at(i)->SetDieStateFromParent(this);
	}
	//DEBUG_PRINT("/_/_/TaskUnit解放 name : %s /_/_/\n",m_name.c_str());
}

/* ================================================ */
/**
 * @brief	親タスク設定
 */
/* ================================================ */
void TaskUnit::TaskStartDie()
{
	if( GetStatus() < TASK_PRE_DIE){
		SetStatus(TASK_PRE_DIE);
	}
}

/* ================================================ */
/**
 * @brief	親タスク設定
 */
/* ================================================ */
void TaskUnit::AddConnectionParent( TaskUnit *parent )
{
	//! 親に設定される
	if(!m_pParent){
		m_pParent = parent;
	}
	else{
		//! すでにある場合はアサート
		DEBUG_ASSERT( 0, "親がすでにあるのにまたセットしようとしている\n");
	}
}

/* ================================================ */
/**
 * @brief	子タスク設定
 */
/* ================================================ */
void TaskUnit::SetChildUnit( TaskUnit *unit )
{
	unit->AddConnectionParent(this);
	m_pChildVec.push_back(unit);
}

/* ================================================ */
/**
 * @brief	親子関係の解除
 */
/* ================================================ */
void TaskUnit::ReleaseConnection( TaskUnit *unit )
{
	if(m_pParent == unit){
		m_pParent = NULL;
		return;
	}

	std::vector<TaskUnit*>::iterator it;
	for(it = m_pChildVec.begin(); it != m_pChildVec.end(); ++it){
		if((*it) == unit){
			m_pChildVec.erase(it);
			return;
		}
	}

	DEBUG_ASSERT( 0, "親子関係がおかしい、ReleaseConnection引数が見当たらない。\n");
}

/* ================================================ */
/**
 * @brief	親にメッセージを投げる
 */
/* ================================================ */
void TaskUnit::SendMessageToParent( const Message &msg )
{
	if( m_pParent ){
		m_pParent->MessageReceive(msg);
	}
}

/* ================================================ */
/**
 * @brief	ステータス変更
 */
/* ================================================ */
void TaskUnit::SetStatus( const TASK_STATUS &status )
{
	m_status = status;
}

void TaskUnit::SetDieStateFromParent( TaskUnit *pParent )
{
	if( !m_pParent || m_pParent != pParent ){
		DEBUG_ASSERT( 0, "親以外から呼ばれたかそもそも親がない\n");
	}

	//! 親子関係を解除して死ぬ準備
	ReleaseConnection( pParent );
	if( GetStatus() < TASK_PRE_DIE ){
		SetStatus( TASK_PRE_DIE );
	}
}
