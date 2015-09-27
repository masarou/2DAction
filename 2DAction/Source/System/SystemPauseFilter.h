/* ====================================================================== */
/**
 * @brief  Pause時画面を覆う黒板
 *
 * @note
 *		
 */
/* ====================================================================== */
#ifndef __PAUSE_FILTER__
#define __PAUSE_FILTER__

#include "Game/Game2DBase.h"
#include "System/Task/SystemTaskUnit.h"

class PauseFilter : public TaskUnit
{

public:

	static PauseFilter *Create();
	static PauseFilter *GetInstance();
	PauseFilter(void);
	~PauseFilter(void);

	// 情報取得
	void	Show();
	void	Hide();

protected:

	virtual bool DieMain() override;
	virtual void DrawUpdate() override;

private:

	static PauseFilter	*s_pInstance;

	Texture2D			m_drawTexture;	// 表示画像データ
	Texture2D			m_drawPauseStr;	// Pause画像データ

};
#endif // FLOW_FADE_MANAGER