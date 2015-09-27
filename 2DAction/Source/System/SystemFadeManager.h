/* ====================================================================== */
/**
 * @brief  画面を覆うフェードの管理クラス
 *
 * @note
 *		
 */
/* ====================================================================== */
#ifndef __FLOW_FADE_MANAGER__
#define __FLOW_FADE_MANAGER__

#include "Game/Game2DBase.h"
#include "System/Task/SystemTaskUnit.h"

class FadeManager : public TaskUnit
{

public:

	enum STATE_FADE{
		STATE_IDLE,			// 何もしていない(非表示)
		STATE_FADEIN,		// フェードイン中
		STATE_OVERWINDOW,	// 画面に表示中
		STATE_FADEOUT,		// フェードアウト中

		STATE_MAX,
	};

	static FadeManager *Create();
	static FadeManager *GetInstance();
	FadeManager(void);
	~FadeManager(void);

	void	StartFadeIn();		// フェードイン開始
	void	StartFadeOut();		// フェードアウト開始
	const STATE_FADE &GetCurrentState() const{ return m_currState; }	// 現在のフェード状態取得

protected:

	// 継承関数
	virtual bool DieMain() override;
	virtual void DrawUpdate() override;

private:

	static FadeManager		*s_pInstance;

	Game2DBase			*m_fadePlate2D;		// ライフ描画クラス
	TEX_DRAW_INFO		m_PlateInfo;		// ライフ描画情報

	STATE_FADE			m_currState;		// 現在の状態

};
#endif // FLOW_FADE_MANAGER