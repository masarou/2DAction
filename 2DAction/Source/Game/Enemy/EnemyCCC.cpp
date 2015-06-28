/* ====================================================================== */
/**
 * @brief  ìGä«óùÉNÉâÉX
 *
 * @note
 *		
 */
/* ====================================================================== */

#include "EnemyCCC.h"
#include "Game/GameMap.h"
#include "Game/GameRegister.h"

EnemyCCC *EnemyCCC::Create( const uint32_t &enemyLevel, const uint32_t &uniqueID )
{
	return NEW EnemyCCC( enemyLevel, uniqueID );
}

EnemyCCC::EnemyCCC( const uint32_t &enemyLevel, const uint32_t &uniqueID )
	: EnemyBase( "EnemyTough.json", uniqueID, Common::ENEMY_KIND_CCC, enemyLevel )
{
}

EnemyCCC::~EnemyCCC(void)
{
}

bool EnemyCCC::InitMain()
{
	return true;
}

uint32_t EnemyCCC::GetEnemyDefaultHP() const
{
	return 1000 + ( GetEnemyLevel() * 100 );
}

uint32_t EnemyCCC::GetEnemyDefaultSPD() const
{
	return 1 + ( GetEnemyLevel() / 4 );
}