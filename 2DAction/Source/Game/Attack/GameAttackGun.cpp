/* ====================================================================== */
/**
 * @brief  �v���C���[�̍U���Ǘ��N���X
 *
 * @note
 *		
 */
/* ====================================================================== */

#include "GameAttackGun.h"
#include "Bullet.h"
#include "System/Sound/SystemSoundManager.h"

AttackGun *AttackGun::CreateGun( const Common::OWNER_TYPE &ownerType )
{
	return NEW AttackGun( ownerType );
}

AttackGun::AttackGun( const Common::OWNER_TYPE &ownerType )
: TaskUnit("AttackGun")
, m_intervalTime( 0 )
, m_owner( ownerType )
{
	m_currState.Init();
}


AttackGun::~AttackGun(void)
{
}

bool AttackGun::DieMain()
{
	m_magazine.clear();
	return true;
}

/* ================================================ */
/**
 * @brief	�ʒu�A�`��X�V�֐�
 */
/* ================================================ */
void AttackGun::Update()
{
	// ���̒e���˂܂ł̎��Ԃ����Z
	if( m_intervalTime > 0){
		--m_intervalTime;
	}
}

/* ================================================ */
/**
 * @brief	�e�̔���
 */
/* ================================================ */
void AttackGun::ShootBullet( const math::Vector2 &pos, const math::Vector2 &vec )
{
	ShootBullet( pos, vec, m_currState.m_damage, m_currState.m_speed );
}
void AttackGun::ShootBullet( const math::Vector2 &pos, const math::Vector2 &vec, const uint32_t &damage, const uint32_t &speed )
{
	if( m_intervalTime == 0 ){
		Bullet *bul = NEW Bullet( m_owner, pos, vec, damage, speed );
		m_magazine.push_back( bul );
		
		// ���ˉ���炷
		SoundManager::GetInstance()->PlaySE("ShootBullet", 9000);

		// ���Ԋu�̎��Ԃ�݂���
		m_intervalTime += m_currState.m_shootInterval;
	}
}