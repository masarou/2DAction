/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		フローの共通項目、json読み取り等の機能を持たせたクラス
 */
/* ====================================================================== */
#ifndef FLOW_BASE
#define FLOW_BASE

#include "System/SystemDefine.h"
#include "System/Task/SystemTaskManager.h"
#include "System/Task/SystemTaskUnit.h"

class FlowBase : public TaskManagerBase
{
public:

	FlowBase(std::string fileName);
	virtual ~FlowBase(void);
	
	//! 初期化処理記入
	virtual bool Init(){return true;}

	//! 初期化処理記入
	virtual bool Finish();

	//! 毎フレーム呼ばれる
	virtual void FlowUpdate() = 0;
	void ChildUpdate();

	//! タスク追加
	void AddChildTask(TaskUnit *pTask);

	//! json読み込み
	void LoadFlowFile();

protected:

	void StartFade(const char *eventStr);

private:

	struct FLOW_DATA{
		std::string eventStr;	//!< イベント名
		std::string filePath;	//!< イベントに対するファイル名
	};

	std::string m_fileName;		//!< json名
	std::vector<FLOW_DATA> m_vEventName;
};
#endif

