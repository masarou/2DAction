/* ====================================================================== */
/**
 * @brief  Pause時画面を覆う黒板
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
	m_drawTexture.m_pTex2D = Game2DBase::Create( "PauseFilter.json" );
	TEX_DRAW_INFO drawInfo;
	drawInfo.Init();
	const TEX_INIT_INFO &info	= TextureResourceManager::GetInstance()->GetLoadTextureInfo( "PauseFilter.json" );
	drawInfo.m_prioity			= Common::PRIORITY_HIGH;
	drawInfo.m_scale			= math::Vector2( (WINDOW_WIDTH / info.m_sizeWidth) + 1.0f, (WINDOW_HEIGHT / info.m_sizeHeight) + 1.0f );
	drawInfo.m_arrangeOrigin	= math::Vector2( 0.0f, 0.0f );
	drawInfo.m_posOrigin		= math::Vector2( 0.0f, 0.0f );
	drawInfo.m_usePlayerOffset	= false;
	m_drawTexture.m_pTex2D->SetDrawInfo( drawInfo );
	
	m_drawPauseStr.Init();
	m_drawPauseStr.m_pTex2D = Game2DBase::Create( "PauseStr.json" );
	TEX_DRAW_INFO drawInfoPause;
	drawInfoPause.Init();
	drawInfoPause.m_prioity			= Common::PRIORITY_HIGH;
	drawInfoPause.m_posOrigin		= WINDOW_CENTER;
	drawInfoPause.m_usePlayerOffset	= false;
	m_drawPauseStr.m_pTex2D->SetDrawInfo( drawInfoPause );
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
	else if( Utility::IsShowPauseFilter() ){
		// 何らかの演出中で黒板のみ必要
		m_drawTexture.m_pTex2D->DrawUpdate2D();
	}
}