
/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		FPS管理クラス
 */
/* ====================================================================== */


#include "SystemFPSManager.h"

FpsManager *FpsManager::s_pInstance = NULL;
uint32_t FpsManager::m_fps = 60;
uint32_t FpsManager::m_updateCounter = 0;

// 内部計算用
namespace{
static uint32_t s_instancCreateTime = 0;	// インスタンス生成時の時間
static uint32_t s_preUpdateTime = 0;		// 前のフレームの更新時間
}

/* ================================================ */
/**
 * @brief	コンストラクタ
 */
/* ================================================ */
FpsManager::FpsManager()
{
}

/* ================================================ */
/**
 * @brief	デストラクタ
 */
/* ================================================ */
FpsManager::~FpsManager()
{
	s_pInstance = NULL;
}

/* ================================================ */
/**
 * @brief	生成
 */
/* ================================================ */
FpsManager* FpsManager::Create()
{
	if( s_pInstance == NULL ){
		s_pInstance = NEW FpsManager;

		s_instancCreateTime = GetNowCount();	// アプリ開始時の時間
		s_preUpdateTime = s_instancCreateTime;
	}

	return s_pInstance;
}

/* ================================================ */
/**
 * @brief	インスタンス取得
 */
/* ================================================ */
FpsManager* FpsManager::GetInstance()
{
	return s_pInstance;
}

/* ================================================ */
/**
 * @brief	破壊
 */
/* ================================================ */
void FpsManager::Destroy()
{
	SAFE_DELETE( s_pInstance );
}

/* ================================================ */
/**
 * @brief	フレーム更新タイミングかチェック
 */
/* ================================================ */
bool FpsManager::IsUpdateTiming()
{
	uint32_t nowTime = GetNowCount();

	//! インスタンス作成からのフレーム
	uint32_t nowFrame = (nowTime - s_instancCreateTime)*m_fps/1000;
	//! 前回更新時のフレーム
	uint32_t framePreUpdate = (s_preUpdateTime - s_instancCreateTime)*m_fps/1000;

	if( nowFrame != framePreUpdate ){
		//! 更新時間を更新
		s_preUpdateTime = nowTime;
		++m_updateCounter;
		return true;
	}else{
		//! まだフレーム更新タイミングでない
		return false;
	}
}

/* ================================================ */
/**
 * @brief	FPSの設定
 */
/* ================================================ */
void FpsManager::SetFps( uint32_t fps )
{
	m_fps = fps;
}

/* ================================================ */
/**
 * @brief	FPSの取得
 */
/* ================================================ */
const uint32_t &FpsManager::GetFps()
{
	return m_fps;
}

/* ================================================ */
/**
 * @brief	FPSの取得 float
 */
/* ================================================ */
float FpsManager::GetFpsF()
{
	return static_cast<float>(m_fps);
}

/* ================================================ */
/**
 * @brief	ゲームの更新カウンタ取得
 */
/* ================================================ */
const uint32_t FpsManager::GetUpdateCounter()
{
	return m_updateCounter;
}
