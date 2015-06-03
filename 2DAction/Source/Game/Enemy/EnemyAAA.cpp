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
	: EnemyBase( "EnemySlime.json", uniqueID, Common::ENEMY_KIND_AAA )
{
}

EnemyAAA::~EnemyAAA(void)
{
}

bool EnemyAAA::InitMain()
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

uint32_t EnemyAAA::GetEnemyDefaultHP() const
{
	// �X�e�[�W�ɂ���čő僉�C�t�ύX
	return 50 + ( 100 * ( GetEnemyLevel() - 1 ) );
}
