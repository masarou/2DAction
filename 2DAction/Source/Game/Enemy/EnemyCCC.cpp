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

EnemyCCC *EnemyCCC::Create( const uint32_t &enemyLevel, const uint32_t &uniqueID, const math::Vector2 &enemyPos )
{
	return NEW EnemyCCC( enemyLevel, uniqueID, enemyPos );
}

EnemyCCC::EnemyCCC( const uint32_t &enemyLevel, const uint32_t &uniqueID, const math::Vector2 &enemyPos )
	: EnemyBase( "EnemyTough.json", uniqueID, Common::ENEMY_KIND_COW, enemyLevel, enemyPos )
{
}

EnemyCCC::~EnemyCCC(void)
{
}

bool EnemyCCC::InitMain()
{
	return true;
}

uint32_t const EnemyCCC::GetEnemyDefaultHP() const
{
	return 1000 + ( GetEnemyLevel() * 100 );
}

uint32_t const EnemyCCC::GetEnemyDefaultSPD() const
{
	return 1 + ( GetEnemyLevel() / 4 );
}