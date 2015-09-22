/* ====================================================================== */
/**
 * @brief  敵管理クラス
 *
 * @note
 *		
 */
/* ====================================================================== */

#include "EnemySlime.h"
#include "Game/GameMap.h"
#include "Game/GameRegister.h"

EnemySlime *EnemySlime::Create( const uint32_t &enemyLevel, const uint32_t &uniqueID, const math::Vector2 &enemyPos )
{
	std::string fileStr = "";
	switch(enemyLevel){
	case 1:
	case 2:
		fileStr = "EnemySlime01.json";
		break;
	case 3:
	case 4:
		fileStr = "EnemySlime02.json";
		break;
	case 5:
	case 6:
		fileStr = "EnemySlime03.json";
		break;
	case 7:
	case 8:
	case 9:
		fileStr = "EnemySlime04.json";
		break;
	default:
		DEBUG_ASSERT( 0, "スライムのレベルが想定外");
		fileStr = "EnemySlime01.json";
		break;
	}
	return NEW EnemySlime( fileStr, enemyLevel, uniqueID, enemyPos );
}

EnemySlime::EnemySlime( const std::string &fileName, const uint32_t &enemyLevel, const uint32_t &uniqueID, const math::Vector2 &enemyPos )
	: EnemyBase( fileName, uniqueID, Common::ENEMY_KIND_SLIME, enemyLevel, enemyPos )
{
}

EnemySlime::~EnemySlime(void)
{
}

bool EnemySlime::InitMain()
{

	return true;
}

 const uint32_t EnemySlime::GetEnemyDefaultHP() const
{
	// Lvによって最大ライフ変更
	return 50 + ( 100 * ( GetEnemyLevel() - 1 ) );
}

 const uint32_t EnemySlime::GetEnemyDefaultSPD() const
{
	return 1 + ( GetEnemyLevel() / 3 );
}
