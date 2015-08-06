/* ====================================================================== */
/**
 * @brief  “GŠÇ—ƒNƒ‰ƒX
 *
 * @note
 *		
 */
/* ====================================================================== */

#include "EnemyBoss.h"
#include "Game/GameMap.h"
#include "Game/GameRegister.h"

EnemyBoss *EnemyBoss::Create( const uint32_t &uniqueID )
{
	return NEW EnemyBoss( uniqueID );
}

EnemyBoss::EnemyBoss( const uint32_t &uniqueID )
	: EnemyBase( "EnemyBoss.json", uniqueID, Common::ENEMY_KIND_BOSS, 0 )
{
}

EnemyBoss::~EnemyBoss(void)
{
}





EnemySlimeKing *EnemySlimeKing::Create( const uint32_t &uniqueID )
{
	return NEW EnemySlimeKing( uniqueID );
}

EnemySlimeKing::EnemySlimeKing( const uint32_t &uniqueID )
	: EnemyBase( "EnemySlimeKing.json", uniqueID, Common::ENEMY_KIND_SLIME_KING, 0 )
{
}

EnemySlimeKing::~EnemySlimeKing(void)
{
}

const uint32_t EnemySlimeKing::GetEnemyDefaultSPD() const
{
	if( m_pEnemyAI && m_pEnemyAI->GetAIKind() == Common::AI_MOVE_PLAYER_SLIME_KING ){
		return 3;
	}
	return 1;
}
