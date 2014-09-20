/* ====================================================================== */
/**
 * @brief  “GŠÇ—ƒNƒ‰ƒX
 *
 * @note
 *		
 */
/* ====================================================================== */

#include "EnemyAAA.h"
#include "Common/Utility/CommonGameUtility.h"

EnemyAAA *EnemyAAA::Create( uint32_t uniqueID )
{
	return NEW EnemyAAA( uniqueID );
}

EnemyAAA::EnemyAAA( uint32_t uniqueID )
	: EnemyBase( "player.json", uniqueID, Common::KIND_AAA )
{
}

EnemyAAA::~EnemyAAA(void)
{
}

void EnemyAAA::UpdateEnemy()
{
	m_enemyInfo.m_pos += 0.5f;
	m_enemyInfo.m_offset = gameUtility::GetPlayerOffsetPos();
	m_enemy2D->SetDrawInfo( m_enemyInfo );
}

