/* ====================================================================== */
/**
 * @brief  �G�Ǘ��N���X
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
	: EnemyBase( "player.json", 0, Common::KIND_AAA )
{
}

EnemyAAA::~EnemyAAA(void)
{
}

void EnemyAAA::Update()
{
	m_enemyInfo.m_pos += 0.5f;
	m_enemyInfo.m_offset = gameUtility::GetPlayerOffsetPos();
	m_enemy2D->SetDrawInfo( m_enemyInfo );
}

