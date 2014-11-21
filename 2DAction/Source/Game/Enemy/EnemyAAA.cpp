/* ====================================================================== */
/**
 * @brief  ìGä«óùÉNÉâÉX
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
