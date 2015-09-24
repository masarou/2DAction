
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

#ifndef __SYSTEM_TASK_MANAGER__
#define __SYSTEM_TASK_MANAGER__

#include <vector>
#include "System/SystemDefine.h"

class TaskUnit;

class TaskManagerBase
{
public:

	TaskManagerBase();
	virtual ~TaskManagerBase(void);

	//! ユニットの登録
	void AddUnit( TaskUnit *unit );

	//! 全タスク終了
	bool ReleaseAllEnd();

protected:

	//! 更新処理
	void Exec();
	void DrawUpdate();

	void DeleteDieUnit();
	std::vector<TaskUnit*> m_vTaskUnit;

};

class TaskManager : public TaskManagerBase
{
public:

	virtual ~TaskManager(void){};
	static TaskManager *GetInstance(){return m_pInstance;}

	void ExecApp(){
		Exec();
		DrawUpdate();
	}

private:

	TaskManager(){};
	static TaskManager *m_pInstance;

};

#endif