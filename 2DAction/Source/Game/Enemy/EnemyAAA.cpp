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
	: EnemyBase( "player.json", uniqueID, Common::KIND_AAA )
{
}

EnemyAAA::~EnemyAAA(void)
{
}

bool EnemyAAA::InitMain()
{
	// �����ʒu�Z�b�g
	const GameMap *pMap = GameRegister::GetInstance()->GetGameMap();
	for(;;){
		math::Vector2 pos = math::Vector2( GetRandamValue( 3000, 0 ), GetRandamValue( 3000, 0 ));
		if( pMap->GetTileHeight(pos) == 0){
			m_textureEnemy.m_texInfo.m_pos = pos;
			DEBUG_PRINT( "�G���� x = %f, y = %f\n", pos.x, pos.y );
			break;
		}
	}
	return true;
}
