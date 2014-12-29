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

EnemyAAA *EnemyAAA::Create( const uint32_t &uniqueID )
{
	return NEW EnemyAAA( uniqueID );
}

EnemyAAA::EnemyAAA( const uint32_t &uniqueID )
	: EnemyBase( "player.json", uniqueID, Common::ENEMY_KIND_AAA )
{
}

EnemyAAA::~EnemyAAA(void)
{
}

bool EnemyAAA::InitMain()
{
	// 初期位置セット
	m_textureEnemy.m_texInfo.m_posOrigin = GetMapRandamPos( /*allowInWindow=*/false );
	DEBUG_PRINT( "敵生成 x = %f, y = %f\n", m_textureEnemy.m_texInfo.m_posOrigin.x, m_textureEnemy.m_texInfo.m_posOrigin.y );
	return true;
}
