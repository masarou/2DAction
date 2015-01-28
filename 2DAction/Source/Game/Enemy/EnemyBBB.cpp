/* ====================================================================== */
/**
 * @brief  �G�Ǘ��N���X
 *
 * @note
 *		
 */
/* ====================================================================== */

#include "EnemyBBB.h"
#include "Game/GameMap.h"
#include "Game/GameRegister.h"

EnemyBBB *EnemyBBB::Create( const uint32_t &uniqueID )
{
	return NEW EnemyBBB( uniqueID );
}

EnemyBBB::EnemyBBB( const uint32_t &uniqueID )
: EnemyBase( "player.json", uniqueID, Common::ENEMY_KIND_BBB )
{
}

EnemyBBB::~EnemyBBB(void)
{
}

bool EnemyBBB::InitMain()
{
	// �����ʒu�Z�b�g
	m_drawTexture.m_texInfo.m_posOrigin = GetMapRandamPos( /*allowInWindow=*/false );
	DEBUG_PRINT( "�G���� x = %f, y = %f\n", m_drawTexture.m_texInfo.m_posOrigin.x, m_drawTexture.m_texInfo.m_posOrigin.y );

	return true;
}

/* ================================================ */
/**
 * @brief	���̃N���X����̃C�x���g�R�[��
 */
/* ================================================ */
void EnemyBBB::EventUpdate( const Common::CMN_EVENT &eventId )
{
	EnemyBase::EventUpdate( eventId );

	switch( eventId.m_event ){
	case Common::EVENT_SHOOT_BULLET:
		if( s_pAttackGun ){
			math::Vector2 direction = GetPlayerPos() - GetDrawInfo().m_posOrigin;
			direction.Normalize();
			s_pAttackGun->ShootBullet( GetDrawInfo().m_posOrigin, direction );
		}
		break;
	default:
		break;
	}

}