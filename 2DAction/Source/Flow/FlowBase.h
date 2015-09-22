/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		フローの共通項目、json読み取り等の機能を持たせたクラス
 */
/* ====================================================================== */
#ifndef __FLOW_BASE__
#define __FLOW_BASE__

#include "System/SystemDefine.h"
#include "System/Task/SystemTaskManager.h"
#include "System/Task/SystemTaskUnit.h"
#include "System/Input/SystemInputWatcher.h"
#include "System/Sound/SystemSoundManager.h"
#include "FlowManager.h"

class ProcessBase;

class FlowBase : public TaskManagerBase, public InputWatcher 
{
	friend class FlowManager;

protected:

	FlowBase( const std::string &fileName);
	virtual ~FlowBase(void);
	
	//! 初期化処理記入
	virtual bool Init(){return true;}

	//! 派生先での更新関数
	virtual void UpdateFlowPreChildTask(){}
	virtual void CheckNextFlow(){}
	virtual void UpdateFlowAfterChildTask(){}

	//! 派生先終了処理記入
	virtual bool FinishFlow(){ return true; }

	//! 現在のフロー名取得
	const std::string &GetFlowFilePath() const{ return m_filePath; }

	//! ステージエフェクトAdd
	void PushStageEffect( ProcessBase *pEffect ){ m_vStageEffect.push_back( pEffect ); }
	bool IsPlaySpecialEffect(){ return ( m_vStageEffect.size() == 0 ) ? false : true ; }

protected:
	
	// 次の画面に遷移
	bool StartFade( const char *eventStr );

	// ゲームをポーズ状態に
	virtual void PadEventStart() override;

private:

	//!	終了処理記入
	bool Finish();

	//! json読み込み
	void LoadFlowFile();
	
	//! 毎フレーム呼ばれる
	void UpdateFlow();

	//! タスク追加
	void AddChildTask(TaskUnit *pTask);

	//! ぶら下がっている子の更新
	void ChildUpdate();


	struct FLOW_DATA{
		std::string eventStr;	//!< イベント名
		std::string filePath;	//!< イベントに対するファイル名
	};

	bool m_isInvalidPadCtrl;	//!< パッド操作無効フラグ
	std::string m_filePath;		//!< 読み込んでいるjsonファイルパス
	std::vector<FLOW_DATA> m_vEventName;

	std::vector<ProcessBase*>	m_vStageEffect;	// ゲーム全体を止めて見せる演出(ゲーム説明等)
};
#endif

