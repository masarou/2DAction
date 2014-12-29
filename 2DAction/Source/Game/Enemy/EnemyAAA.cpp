/* ====================================================================== */
/**
 * @brief  �G�Ǘ��N���X
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
	// �����ʒu�Z�b�g
	m_textureEnemy.m_texInfo.m_posOrigin = GetMapRandamPos( /*allowInWindow=*/false );
	DEBUG_PRINT( "�G���� x = %f, y = %f\n", m_textureEnemy.m_texInfo.m_posOrigin.x, m_textureEnemy.m_texInfo.m_posOrigin.y );
	return true;
}
