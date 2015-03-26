/* ====================================================================== */
/**
 * @brief  �G�Ǘ��N���X
 *
 * @note
 *		
 */
/* ====================================================================== */

#include "EnemyCCC.h"
#include "Game/GameMap.h"
#include "Game/GameRegister.h"

EnemyCCC *EnemyCCC::Create( const uint32_t &uniqueID )
{
	return NEW EnemyCCC( uniqueID );
}

EnemyCCC::EnemyCCC( const uint32_t &uniqueID )
	: EnemyBase( "enemyTough.json", uniqueID, Common::ENEMY_KIND_CCC )
{
}

EnemyCCC::~EnemyCCC(void)
{
}

bool EnemyCCC::InitMain()
{
	// �����ʒu�Z�b�g
	for(;;){
		math::Vector2 candidatePos = Utility::GetMapRandamPos( /*allowInWindow=*/false );
		// �}�b�v��̓����鍂���Ȃ琶��
		if( Utility::GetMapHeight( candidatePos ) <= GetWalkHeight() ){
			m_drawTexture.m_texInfo.m_posOrigin = candidatePos;
			DEBUG_PRINT( "�G���� x = %f, y = %f\n", m_drawTexture.m_texInfo.m_posOrigin.x, m_drawTexture.m_texInfo.m_posOrigin.y );
			break;
		}
	}
	return true;
}
