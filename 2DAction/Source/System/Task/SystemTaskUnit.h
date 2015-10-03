/* ====================================================================== */
/**
 * @brief  各クラスの最小単位のクラス
 *
 * @note
 *		基本的にはこのクラスを派生させたものを使用する。
 *		m_statusによって自動的にupdateが呼ばれたりdeleteされたりする。
 *		setStatusでTASK_PRE_DIEにしてやると自動的に削除される。
 *		関数説明
 *			Init()		: 最初に必ず呼ばれる、trueを返すまで呼ばれ続ける
 *			Update()	: 毎フレーム呼ばれる、更新処理等に
 *			DrawUpdate(): 毎フレーム呼ばれる、描画等に
 *			DieMain()	: m_statusをUNIT_DEADにしたら呼ばれる。
 *						  trueを返すまで呼ばれ続け、その後クラスが解放される
 *
 *		SetChildUnit()で親子付けしたTaskUnit継承クラスは親が死ぬと自動的に死にます
 */
/* ====================================================================== */

#ifndef __SYSTEM_TASK_UNIT__
#define __SYSTEM_TASK_UNIT__

#include <string>
#include <vector>
#include "System/SystemDefine.h"
#include "Common/CommonDefine.h"
#include "System/SystemMessage.h"

class TaskManager;

class TaskUnit
{
	friend class TaskManagerBase;
	friend class FlowBase;

public:

	//! タスクのステータス
	enum TASK_STATUS{
		TASK_IDLE,		//!< 何もしない(ポーズ等)
		TASK_INIT,		//!< 初期化
		TASK_ALIVE,		//!< 普通の状態
		TASK_PRE_DIE,	//!< CanDieが呼ばれる前
		TASK_DIE,		//!< CanDieが呼ばれた後

		TASK_MAX
	};

	const std::string &GetName(){return m_name;}
	const TASK_STATUS &GetStatus() const;

	// もう死んでいるorこれから死ぬかどうか
	bool IsDie();

	//! 基本的に使用禁止
	void SetDieStateFromParent( TaskUnit *pParent );

protected:

	TaskUnit( std::string name = "No Name", TASK_STATUS status = TASK_INIT );
	virtual ~TaskUnit(void);

	virtual bool Init(){return true;}
	virtual void Update(){};			// 移動等の内部数値の更新
	virtual void DrawUpdate(){};		// 描画更新
	virtual bool DieMain(){return true;}

	// 派生先でのメッセージ処理
	virtual void MessageReceive( const Message &msg ){}

	// 死ぬ時はこれを呼んで下さい
	void TaskStartDie();

	//! 親クラスの登録
	void AddConnectionParent( TaskUnit *unit );

	//! 子クラスの登録
	void SetChildUnit( TaskUnit *parent );

	//! 親子関係の解除
	void ReleaseConnection( TaskUnit *unit );

	//! 親にメッセージを投げる
	void SendMessageToParent( const Message &msg );

private:

	//! ステータス変更
	void SetStatus( const TASK_STATUS &status );

	//! クラスの状態
	std::string m_name;
	TASK_STATUS m_status;

	//! 親クラス
	TaskUnit *m_pParent;

	//! 子クラス
	std::vector<TaskUnit*> m_pChildVec;

};

#endif //SYSTEM_TASK_UNIT
