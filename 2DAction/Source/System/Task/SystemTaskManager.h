
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

#ifndef SYSTEM_TASK_MANAGER
#define SYSTEM_TASK_MANAGER

#include <vector>
#include "System/SystemDefine.h"
#include "System/SystemMessage.h"

class TaskUnit;

class TaskManagerBase
{
public:

	TaskManagerBase();
	virtual ~TaskManagerBase(void);

	//! ユニットの登録
	void AddUnit(TaskUnit *unit);

	//! 更新処理
	void Update();
	void EventUpdate();
	void DrawUpdate();

	//! 全タスク終了
	bool ReleaseAllEnd();

protected:

	void DeleteDieUnit();
	std::vector<TaskUnit*> m_vTaskUnit;

};

class TaskManager : public TaskManagerBase
{
public:

	virtual ~TaskManager(void){};
	static TaskManager *GetInstance(){return m_pInstance;}

private:

	TaskManager(){};
	static TaskManager *m_pInstance;

};

#endif