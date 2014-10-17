/* ====================================================================== */
/**
 * @brief  “GŠÇ—ƒNƒ‰ƒX
 *
 * @note
 *		
 */
/* ====================================================================== */

#include "EnemyAAA.h"

EnemyAAA *EnemyAAA::Create( const uint32_t &uniqueID )
{
	return NEW EnemyAAA( uniqueID );
}

EnemyAAA::EnemyAAA( const uint32_t &uniqueID )
	: EnemyBase( "player.json", uniqueID, Common::KIND_AAA )
{
	m_HP = 1;
}

EnemyAAA::~EnemyAAA(void)
{
}

void EnemyAAA::UpdateEnemy()
{
	m_enemyInfo.m_pos += 0.5f;
	m_enemy2D->SetDrawInfo( m_enemyInfo );
}

