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

EnemyAAA *EnemyAAA::Create( const uint32_t &enemyLevel, const uint32_t &uniqueID )
{
	return NEW EnemyAAA( enemyLevel, uniqueID );
}

EnemyAAA::EnemyAAA( const uint32_t &enemyLevel, const uint32_t &uniqueID )
	: EnemyBase( "EnemySlime.json", uniqueID, Common::ENEMY_KIND_AAA, enemyLevel )
{
}

EnemyAAA::~EnemyAAA(void)
{
}

bool EnemyAAA::InitMain()
{

	return true;
}

uint32_t EnemyAAA::GetEnemyDefaultHP() const
{
	return 50;
	// ステージによって最大ライフ変更
	return 50 + ( 100 * ( GetEnemyLevel() - 1 ) );
}

uint32_t EnemyAAA::GetEnemyDefaultSPD() const
{
	return 1 + ( GetEnemyLevel() / 3 );
}
