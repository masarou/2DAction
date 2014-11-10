/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		フローのすべてを管理するManager
 */
/* ====================================================================== */
#ifndef FLOW_MANAGER
#define FLOW_MANAGER

#include "System/Task/SystemTaskUnit.h"
#include "Flow/FlowBase.h"

class FlowManager : public TaskUnit
{
	friend FlowBase;

public:

	static void Create();
	static FlowManager *GetInstance();

	//! ユニットの登録
	bool AddUnit(TaskUnit *unit);

private:

	//! 管理フローのステップ
	enum FLOW_STATUS{
		FLOW_FADEIN_CHECK,	// フェードインセット
		FLOW_FADEIN_WAIT,	// フェードイン待ち
		FLOW_CHANGE_WAIT,	// 死亡待ち
		FLOW_INIT,			// 初期化前
		FLOW_FADEOUT_CHECK,	// フェードアウトセット
		FLOW_FADEOUT_WAIT,	// フェードアウト待ち
		FLOW_ACTION,		// 起動中

		FLOW_MAX,
	};

	FlowManager();
	~FlowManager();

	virtual bool Init();
	virtual void Update();
	virtual void DrawUpdate();
	virtual bool DieMain();

	//! フロー変更
	void ChangeFlow(const char* filePath);

	static FlowManager	*m_pInstance;
	std::string			m_nextFilePath;	//!<遷移先の画面のファイル名
	FlowBase			*m_pFlow;		//!<今現在表示しているフロー【タイトルやゲーム等】
	FLOW_STATUS			m_step;			//!<フェードアニメの現在のステップ
	FLOW_STATUS			m_fadeNext;		//!<フェードアニメの次のステップ

};
#endif
// FLOW_MANAGER