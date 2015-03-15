/* ====================================================================== */
/**
 * @brief  Pause時画面を覆うフェード
 *
 * @note
 *		
 */
/* ====================================================================== */


#include "SystemPauseFilter.h"
#include "System/Draw2D/SystemDraw2DResource.h"
#include "Common/Utility/CommonGameUtility.h"

PauseFilter *PauseFilter::s_pInstance = NULL;

PauseFilter *PauseFilter::Create()
{
	if( !s_pInstance ){
		s_pInstance = NEW PauseFilter();
	}
	return s_pInstance;
}

PauseFilter *PauseFilter::GetInstance()
{
	return s_pInstance;
}

PauseFilter::PauseFilter()
: TaskUnit("PauseFilter")
{
	// 描画クラスセットアップ
	m_drawTexture.Init();
	m_drawTexture.m_pTex2D = NEW Game2DBase( "pauseFilter.json" );
	m_drawTexture.m_texInfo.m_prioity			= PRIORITY_HIGH;
	const TEX_INIT_INFO &info					= TextureResourceManager::GetInstance()->GetLoadTextureInfo( "pauseFilter.json" );
	m_drawTexture.m_texInfo.m_scale				= math::Vector2( (WINDOW_WIDTH / info.m_sizeWidth) + 1.0f, (WINDOW_HEIGHT / info.m_sizeHeight) + 1.0f );
	m_drawTexture.m_texInfo.m_arrangeOrigin		= math::Vector2( 0.0f, 0.0f );
	m_drawTexture.m_texInfo.m_posOrigin			= math::Vector2( 0.0f, 0.0f );
	m_drawTexture.m_texInfo.m_usePlayerOffset	= false;
	m_drawTexture.m_pTex2D->SetDrawInfo(m_drawTexture.m_texInfo);
	
	m_drawPauseStr.Init();
	m_drawPauseStr.m_pTex2D = NEW Game2DBase( "pauseStr.json" );
	m_drawPauseStr.m_texInfo.m_prioity			= PRIORITY_HIGH;
	m_drawPauseStr.m_texInfo.m_posOrigin		= math::Vector2( WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f );
	m_drawPauseStr.m_texInfo.m_usePlayerOffset	= false;
	m_drawPauseStr.m_pTex2D->SetDrawInfo(m_drawPauseStr.m_texInfo);
}


PauseFilter::~PauseFilter(void)
{
	s_pInstance = NULL;
}

bool PauseFilter::DieMain()
{
	m_drawTexture.DeleteAndInit();
	m_drawPauseStr.DeleteAndInit();
	return true;
}

void PauseFilter::DrawUpdate()
{
	if( Utility::IsGamePause() ){
		// Pause中
		m_drawTexture.m_pTex2D->DrawUpdate2D();
		m_drawPauseStr.m_pTex2D->DrawUpdate2D();
	}
	else if(0){
		// ゲームフローの最初の演出中
		m_drawTexture.m_pTex2D->DrawUpdate2D();
	}
}