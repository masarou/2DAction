/* ====================================================================== */
/**
 * @brief  
 *		フローのすべてを管理するManager
 * @note
 *		
 */
/* ====================================================================== */
#ifndef __FLOW_MANAGER__
#define __FLOW_MANAGER__

#include "Common/CommonDefine.h"
#include "System/Task/SystemTaskUnit.h"
#include "FlowBase.h"
#include "Process/FlowProcessBase.h"

class FlowManager : public TaskUnit
{
	friend class FlowBase;

public:

	static void Create();
	static FlowManager *GetInstance();

	//! ユニットの登録
	bool AddUnit(TaskUnit *unit);

	//! 現在のフロー名取得
	const char *GetCurrentFlow() const;

	//! 現在のゲームフローの種類取得
	const Common::GAME_FLOW &GetCurrentFlowKind() const{ return m_currFlow; }
	const Common::GAME_FLOW &GetPreFlowKind() const{ return m_preFlow; }

	//! ステージエフェクト追加
	void SetupSpecialEffect( ProcessBase *pEffect );

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
	virtual bool DieMain();

	//! フロー変更
	bool ChangeFlow(const char* filePath);

	static FlowManager	*m_pInstance;
	std::string			m_nextFilePath;	//!<遷移先の画面のファイル名
	FlowBase			*m_pFlow;		//!<今現在表示しているフロー【タイトルやゲーム等】
	FLOW_STATUS			m_step;			//!<フェードアニメの現在のステップ
	FLOW_STATUS			m_fadeNext;		//!<フェードアニメの次のステップ

	Common::GAME_FLOW	m_currFlow;
	Common::GAME_FLOW	m_preFlow;
};
#endif
// FLOW_MANAGER