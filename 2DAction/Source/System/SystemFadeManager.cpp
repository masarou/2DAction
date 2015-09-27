/* ====================================================================== */
/**
 * @brief  画面を覆うフェードの管理クラス
 *
 * @note
 *		
 */
/* ====================================================================== */


#include "SystemFadeManager.h"
#include "System/Draw2D/SystemDraw2DResource.h"
#include "System/Sound/SystemSoundManager.h"

FadeManager *FadeManager::s_pInstance = NULL;

FadeManager *FadeManager::Create()
{
	if( !s_pInstance ){
		s_pInstance = NEW FadeManager();
	}
	return s_pInstance;
}

FadeManager *FadeManager::GetInstance()
{
	return s_pInstance;
}

FadeManager::FadeManager()
: TaskUnit("FadeManager")
, m_currState( STATE_IDLE )
{
	// 描画クラスセットアップ
	m_fadePlate2D = Game2DBase::Create("WhiteFade.json");
	m_PlateInfo.Init();
	m_PlateInfo.m_prioity			= Common::PRIORITY_SCENE_FADE;
	const TEX_INIT_INFO &info		= TextureResourceManager::GetInstance()->GetLoadTextureInfo( "WhiteFade.json" );
	m_PlateInfo.m_scale				= math::Vector2( (WINDOW_WIDTH / info.m_sizeWidth) + 1.0f, (WINDOW_HEIGHT / info.m_sizeHeight) + 1.0f );
	m_PlateInfo.m_arrangeOrigin		= math::Vector2( 0.0f, 0.0f );
	m_PlateInfo.m_posOrigin				= math::Vector2( 0.0f, 0.0f );
	m_PlateInfo.m_usePlayerOffset	= false;
	m_fadePlate2D->SetDrawInfo(m_PlateInfo);
}


FadeManager::~FadeManager(void)
{
	s_pInstance = NULL;
}

bool FadeManager::DieMain()
{
	SAFE_DELETE(m_fadePlate2D);
	return true;
}

/* ================================================ */
/**
 * @brief	フェードイン開始
 */
/* ================================================ */
void FadeManager::StartFadeIn()
{
	if( m_currState != STATE_IDLE ){
		DEBUG_ASSERT( 0, "フェードの状態がおかしい");
		return ;
	}

	std::string animLabel = "fadein";
	if( m_fadePlate2D && animLabel.compare( m_fadePlate2D->GetPlayAnim() ) != 0 ){
		m_fadePlate2D->SetAnim("fadein");
	}
	m_currState = STATE_FADEIN;
	SoundManager::GetInstance()->PlaySE("FadeIn");
}

/* ================================================ */
/**
 * @brief	フェードアウト開始
 */
/* ================================================ */
void FadeManager::StartFadeOut()
{
	if( m_currState != STATE_OVERWINDOW
		&& m_currState != STATE_FADEOUT ){
		return ;
	}

	std::string animLabel = "fadeout";
	if( m_fadePlate2D && animLabel.compare( m_fadePlate2D->GetPlayAnim() ) != 0 ){
		m_fadePlate2D->SetAnim("fadeout");
	}
	m_currState = STATE_FADEOUT;
	SoundManager::GetInstance()->PlaySE("FadeOut");
}

/* ================================================ */
/**
 * @brief	描画更新
 */
/* ================================================ */
void FadeManager::DrawUpdate()
{

	m_fadePlate2D->DrawUpdate2D();

	if( m_currState == STATE_FADEIN || m_currState == STATE_FADEOUT ){
		std::string fadein = "over";
		if( fadein.compare( m_fadePlate2D->GetPlayAnim() ) == 0 ){
			m_currState = STATE_OVERWINDOW;
		}

		std::string fadeout = "none";
		if( fadeout.compare( m_fadePlate2D->GetPlayAnim() ) == 0 ){
			m_currState = STATE_IDLE;
		}
	}
}