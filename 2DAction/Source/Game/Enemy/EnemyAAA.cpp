/* ====================================================================== */
/**
 * @brief  敵管理クラス
 *
 * @note
 *		
 */
/* ====================================================================== */

#include "EnemyAAA.h"
#include "Game/GameMap.h"
#include "Game/GameRegister.h"

EnemyAAA *EnemyAAA::Create( const uint32_t &enemyLevel, const uint32_t &uniqueID, const math::Vector2 &enemyPos )
{
	return NEW EnemyAAA( enemyLevel, uniqueID, enemyPos );
}

EnemyAAA::EnemyAAA( const uint32_t &enemyLevel, const uint32_t &uniqueID, const math::Vector2 &enemyPos )
	: EnemyBase( "EnemySlime.json", uniqueID, Common::ENEMY_KIND_AAA, enemyLevel, enemyPos )
{
}

EnemyAAA::~EnemyAAA(void)
{
}

bool EnemyAAA::InitMain()
{

	return true;
}

 const uint32_t EnemyAAA::GetEnemyDefaultHP() const
{
	// Lvによって最大ライフ変更
	return 50 + ( 100 * ( GetEnemyLevel() - 1 ) );
}

 const uint32_t EnemyAAA::GetEnemyDefaultSPD() const
{
	return 1 + ( GetEnemyLevel() / 3 );
}
