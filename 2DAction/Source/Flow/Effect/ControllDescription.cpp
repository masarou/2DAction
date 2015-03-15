/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		‘€ìà–¾‰‰oƒNƒ‰ƒX
 */
/* ====================================================================== */

#include "ControllDescription.h"
#include "System/Sound/SystemSoundManager.h"


ControllDescription *ControllDescription::Create()
{
	return NEW ControllDescription();
}

ControllDescription::ControllDescription(void)
{
}


ControllDescription::~ControllDescription(void)
{
}

bool ControllDescription::CanDie()
{
	m_controllTex.DeleteAndInit();
	return true;
}

bool ControllDescription::Init()
{
	m_controllTex.Init();
	m_controllTex.m_pTex2D = NEW Game2DBase( "pauseStr.json" );
	m_controllTex.m_texInfo.m_prioity			= PRIORITY_HIGH;
	m_controllTex.m_texInfo.m_posOrigin			= math::Vector2( WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f );
	m_controllTex.m_texInfo.m_usePlayerOffset	= false;
	m_controllTex.m_pTex2D->SetDrawInfo(m_controllTex.m_texInfo);

	return true;
}


void ControllDescription::Update()
{
	if( m_controllTex.m_pTex2D ){
		m_controllTex.m_pTex2D->DrawUpdate2D();
	}
}

void ControllDescription::PadEventDecide()
{
	SetStateNext();
	SoundManager::GetInstance()->PlaySE("Decide");
}
