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
#include "System/Input/SystemInputWatcher.h"

class FlowBase : public TaskManagerBase, public InputWatcher 
{
public:

	FlowBase(std::string fileName);
	virtual ~FlowBase(void);
	
	//! 初期化処理記入
	virtual bool Init(){return true;}

	//!	終了処理記入
	virtual bool Finish();

	//! 毎フレーム呼ばれる
	virtual void UpdateFlow();

	//! ぶら下がっている子の更新
	void ChildUpdate();

	//! タスク追加
	void AddChildTask(TaskUnit *pTask);

	//! json読み込み
	void LoadFlowFile();

	//! 現在のフロー名取得
	const std::string &GetFlowFilePath(){ return m_filePath; }

protected:

	void StartFade(const char *eventStr);

private:

	struct FLOW_DATA{
		std::string eventStr;	//!< イベント名
		std::string filePath;	//!< イベントに対するファイル名
	};

	std::string m_filePath;		//!< 読み込んでいるjsonファイルパス
	std::vector<FLOW_DATA> m_vEventName;
};
#endif

