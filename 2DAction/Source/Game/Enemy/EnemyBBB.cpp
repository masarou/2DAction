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

EnemyBBB *EnemyBBB::Create( const uint32_t &enemyLevel, const uint32_t &uniqueID )
{
	return NEW EnemyBBB( enemyLevel, uniqueID );
}

EnemyBBB::EnemyBBB( const uint32_t &enemyLevel, const uint32_t &uniqueID )
: EnemyBase( "EnemyAhriman.json", uniqueID, Common::ENEMY_KIND_BBB, enemyLevel )
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
void EnemyBBB::EventUpdate( const Common::CMN_EVENT &eventId )
{
	EnemyBase::EventUpdate( eventId );

}

uint32_t EnemyBBB::GetEnemyDefaultHP() const
{
	return 100 + ( 100 * ( GetEnemyLevel() - 1 ) );
}

uint32_t EnemyBBB::GetEnemyDefaultSPD() const
{
	return 1 + ( GetEnemyLevel() / 3 );
}