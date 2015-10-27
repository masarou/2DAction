/* ====================================================================== */
/**
 * @brief  敵管理クラス
 *
 * @note
 *		
 */
/* ====================================================================== */

#include "EnemyAhriman.h"
#include "Game/GameMap.h"
#include "Game/GameRegister.h"

EnemyBBB *EnemyBBB::Create( const uint32_t &uniqueID, const uint32_t &enemyLevel, const math::Vector2 &enemyPos )
{
	return NEW EnemyBBB( uniqueID, enemyLevel, enemyPos );
}

EnemyBBB::EnemyBBB( const uint32_t &uniqueID, const uint32_t &enemyLevel, const math::Vector2 &enemyPos )
: EnemyBase( "EnemyAhriman.json", uniqueID, Common::ENEMY_KIND_AHRIMAN, enemyLevel, enemyPos )
{
}

EnemyBBB::~EnemyBBB(void)
{
}

bool EnemyBBB::InitMain()
{
	return true;
}

/* ================================================ */
/**
 * @brief	他のクラスからのイベントコール
 */
/* ================================================ */
void EnemyBBB::EventUpdate( Common::CMN_EVENT &eventId )
{
	EnemyBase::EventUpdate( eventId );

}

static const uint32_t DEFAULT_HP = 100;
static const float DEFAULT_SPEED = 1.0f;
static const uint32_t DEFAULT_DAMAGE_HIT = 10;

const uint32_t EnemyBBB::GetEnemyDefaultHP() const
{
	return DEFAULT_HP + ( 100 * GetEnemyLevel() );
}

const float EnemyBBB::GetEnemyDefaultSPD() const
{
	return DEFAULT_SPEED + ( GetEnemyLevel() / 3 );
}

const uint32_t EnemyBBB::GetPlayerHitDamage() const
{
	return DEFAULT_DAMAGE_HIT + ( 5 * GetEnemyLevel() );
}