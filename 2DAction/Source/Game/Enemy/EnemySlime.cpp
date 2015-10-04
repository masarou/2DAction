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
	case 3:
		fileStr = "EnemySlime01.json";
		break;
	case 4:
	case 5:
	case 6:
		fileStr = "EnemySlime02.json";
		break;
	case 7:
	case 8:
	case 9:
		fileStr = "EnemySlime03.json";
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
	return 50 + ( 150 * ( GetEnemyLevel() - 1 ) );
}

 const float EnemySlime::GetEnemyDefaultSPD() const
{
	return 1.0f + ( static_cast<float>( GetEnemyLevel() ) / 3.0f );
}

const uint32_t EnemySlime::GetPlayerHitDamage() const
{
	return 10 + ( 5 * GetEnemyLevel() );
}

void EnemySlime::ReduceDamage( Common::CMN_EVENT &eventId )
{
	float levelReduce = static_cast<float>( GetEnemyLevel()*0.05f );
	switch( eventId.m_event ){
	case Common::EVENT_HIT_BULLET_PLAYER:
		// マシンガン攻撃は効きにくい( 0.9~4.5 )
		eventId.m_eventValue *= 0.9f - levelReduce;
		break;
	case Common::EVENT_HIT_BLADE_PLAYER:
		// 斬撃はダメージを増やす
		eventId.m_eventValue *= 1.2f - levelReduce;
		break;
	}
}