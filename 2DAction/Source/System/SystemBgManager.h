/* ====================================================================== */
/**
 * @brief	背景管理クラス
 *			jsonに指定してあるIDからBGをセット
 * @note
 *		
 */
/* ====================================================================== */
#ifndef __BG_MANAGER__
#define __BG_MANAGER__

#include "Game/Game2DBase.h"
#include "System/Task/SystemTaskUnit.h"

class BgManager : public TaskUnit
{

public:

	enum BGID{
		BGID_TITLE,
		BGID_STAGE_RESULT,
		BGID_COMMON,		// 汎用
		BGID_INTERVAL,		// インターバル(ゲームの進行度を見て表示変更)
		BGID_NONE,			// 表示なし
		BGID_CURRENT,		// 現状維持

		BGID_MAX,
	};

	static BgManager *Create();
	static BgManager *GetInstance();
	static void ReleaseInstance();

	BgManager(void);
	~BgManager(void);

	// 情報取得
	void	SetNextBg( const std::string &bgId );
	void	SetNextBg( const BGID &bgId );

	// 準備完了確認関数
	bool	IsShowingBG() const;

protected:

	virtual bool DieMain() override;
	virtual void Update() override;
	virtual void DrawUpdate() override;

private:

	enum BG_STEP{
		STEP_IDLE,
		STEP_INIT,
		STEP_CHANGING,
		STEP_WAIT,
		STEP_SHOWING,

		STEP_MAX,
	};
	
	void SetNextStep( const BG_STEP &next ){ m_currStep = next; }
	std::string GetBgJsonName( const BGID &bgId ) const;

	static BgManager	*s_pInstance;
	BG_STEP				m_currStep;		// クラスの状態を表すステップ

	BGID				m_currBGID;		// 現在のID
	BGID				m_nextBGID;		// 次にセットされたID
	Texture2D			m_drawTexture;	// 表示画像データ

};
#endif // __BG_MANAGER__