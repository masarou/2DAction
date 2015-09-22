/* ====================================================================== */
/**
 * @brief  敵管理クラス
 *
 * @note
 *		
 */
/* ====================================================================== */

#include "EnemyBBB.h"
#include "Game/GameMap.h"
#include "Game/GameRegister.h"

EnemyBBB *EnemyBBB::Create( const uint32_t &enemyLevel, const uint32_t &uniqueID, const math::Vector2 &enemyPos )
{
	return NEW EnemyBBB( enemyLevel, uniqueID, enemyPos );
}

EnemyBBB::EnemyBBB( const uint32_t &enemyLevel, const uint32_t &uniqueID, const math::Vector2 &enemyPos )
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

uint32_t const EnemyBBB::GetEnemyDefaultHP() const
{
	return 100 + ( 100 * ( GetEnemyLevel() - 1 ) );
}

uint32_t const EnemyBBB::GetEnemyDefaultSPD() const
{
	return 1 + ( GetEnemyLevel() / 3 );
}