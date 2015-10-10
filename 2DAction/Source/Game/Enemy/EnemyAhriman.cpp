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
	return NEW EnemyBBB( enemyLevel, uniqueID, enemyPos );
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

const uint32_t EnemyBBB::GetEnemyDefaultHP() const
{
	return 100 + ( 120 * GetEnemyLevel() );
}

const float EnemyBBB::GetEnemyDefaultSPD() const
{
	return 1 + ( GetEnemyLevel() / 3 );
}

const uint32_t EnemyBBB::GetPlayerHitDamage() const
{
	return 10 + ( 5 * GetEnemyLevel() );
}