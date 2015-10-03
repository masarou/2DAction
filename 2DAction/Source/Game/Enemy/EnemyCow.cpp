/* ====================================================================== */
/**
 * @brief  ìGä«óùÉNÉâÉX
 *
 * @note
 *		
 */
/* ====================================================================== */

#include "EnemyCow.h"
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

const uint32_t EnemyCCC::GetEnemyDefaultHP() const
{
	return 1000 + ( GetEnemyLevel() * 100 );
}

const float EnemyCCC::GetEnemyDefaultSPD() const
{
	return 1 + ( GetEnemyLevel() / 4 );
}

const uint32_t EnemyCCC::GetPlayerHitDamage() const
{
	return 20 + ( 7 * GetEnemyLevel() );
}