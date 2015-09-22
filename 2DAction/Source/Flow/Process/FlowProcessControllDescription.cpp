/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		‘€ìà–¾‰‰oƒNƒ‰ƒX
 */
/* ====================================================================== */

#include "FlowProcessControllDescription.h"
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
	TEX_DRAW_INFO drawInfo;
	m_controllTex.Init();
	m_controllTex.m_pTex2D = Game2DBase::Create( "PauseStr.json" );
	drawInfo.m_prioity			= PRIORITY_HIGHEST;
	drawInfo.m_posOrigin			= math::Vector2( WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f );
	drawInfo.m_usePlayerOffset	= false;
	m_controllTex.m_pTex2D->SetDrawInfo( drawInfo );

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
