/* ====================================================================== */
/**
 * @brief  �G�Ǘ��N���X
 *
 * @note
 *		
 */
/* ====================================================================== */

#include "EnemyBoss.h"
#include "Game/GameMap.h"
#include "Game/GameRegister.h"

// �{�X���s������ۂ̖ړI�n
math::Vector2 EnemyBoss::BOSS_TARGET_PLACE = math::Vector2( static_cast<float>(INVALID_VALUE), static_cast<float>(INVALID_VALUE) );

EnemyBoss *EnemyBoss::Create( const uint32_t &uniqueID )
{
	return NEW EnemyBoss( uniqueID );
}

EnemyBoss::EnemyBoss( const uint32_t &uniqueID )
	: EnemyBase( "enemyBoss.json", uniqueID, Common::ENEMY_KIND_BOSS )
{
}

EnemyBoss::~EnemyBoss(void)
{
}

bool EnemyBoss::InitMain()
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

/* ================================================ */
/**
 * @brief	���̃N���X����̃C�x���g�R�[��
 */
/* ================================================ */
void EnemyBoss::EventUpdate( const Common::CMN_EVENT &eventId )
{
	EnemyBase::EventUpdate( eventId );

}