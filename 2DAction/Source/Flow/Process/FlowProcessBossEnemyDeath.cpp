/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		ëÄçÏê‡ñæââèoÉNÉâÉX
 */
/* ====================================================================== */

#include "FlowProcessBossEnemyDeath.h"
#include "System/Sound/SystemSoundManager.h"

#include "System/SystemFPSManager.h"
#include "Common/Utility/CommonGameUtility.h"
#include "Game/Effect/GameEffect.h"

ProcessBossEnemyDeath *ProcessBossEnemyDeath::Create( const math::Vector2 &centerPos )
{
	return NEW ProcessBossEnemyDeath( centerPos );
}

ProcessBossEnemyDeath::ProcessBossEnemyDeath( const math::Vector2 &centerPos )
: m_centerPos( centerPos )
{
}


ProcessBossEnemyDeath::~ProcessBossEnemyDeath(void)
{
}

bool ProcessBossEnemyDeath::CanDie()
{
	return true;
}

bool ProcessBossEnemyDeath::Init()
{

	return true;
}


void ProcessBossEnemyDeath::Update()
{
	static uint32_t timer = 0;
	if( Utility::GetRandamValue( 3, 0 ) == 0 && FpsManager::GetUpdateCounter() % 10 == 0 )
	{
		math::Vector2 pos = m_centerPos;
		pos.x += Utility::GetRandamValue( 30, -30 );
		pos.y += Utility::GetRandamValue( 30, -30 );
		GameEffect::CreateEffect( GameEffect::EFFECT_BOMB, pos );
	}
	
	++timer;

	if( timer < 600 ){
		return;
	}

	SetStateNext();
}
