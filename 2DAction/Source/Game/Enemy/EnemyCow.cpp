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

EnemyCCC *EnemyCCC::Create( const uint32_t &uniqueID, const uint32_t &enemyLevel, const math::Vector2 &enemyPos )
{
	return NEW EnemyCCC( uniqueID, enemyLevel, enemyPos );
}

EnemyCCC::EnemyCCC( const uint32_t &uniqueID, const uint32_t &enemyLevel, const math::Vector2 &enemyPos )
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


static const uint32_t DEFAULT_HP = 1000;
static const float DEFAULT_SPEED = 1.0f;
static const uint32_t DEFAULT_DAMAGE_HIT = 20;

const uint32_t EnemyCCC::GetEnemyDefaultHP() const
{
	return DEFAULT_HP + ( GetEnemyLevel() * 120 );
}

const float EnemyCCC::GetEnemyDefaultSPD() const
{
	return DEFAULT_SPEED + ( static_cast<float>( GetEnemyLevel() ) / 4.0f );
}

const uint32_t EnemyCCC::GetPlayerHitDamage() const
{
	return DEFAULT_DAMAGE_HIT + ( 7 * GetEnemyLevel() );
}