/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		FPS管理クラス
 */
/* ====================================================================== */

#ifndef __SYSTEM_FPS_MANAGER__
#define __SYSTEM_FADE_MANAGER__

#include "SystemDefine.h"

class FpsManager
{
public:
	static FpsManager *Create();
	static FpsManager *GetInstance();

	static void Destroy();

	//! フレーム更新タイミングかチェック
	bool IsUpdateTiming();

	//! FPSの設定
	static void SetFps( uint32_t fps );
	//! FPSの取得
	static const uint32_t &GetFps();
	static float GetFpsF();

private:
	FpsManager();
	~FpsManager();

	static FpsManager *s_pInstance;
	static uint32_t m_fps;
};

#endif
