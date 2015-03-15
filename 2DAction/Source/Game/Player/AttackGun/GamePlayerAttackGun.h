/* ====================================================================== */
/**
 * @brief  �v���C���[�̍U���Ǘ��N���X
 *
 * @note
 *		Bullet�N���X���x�N�^�Ŏ���
 *		���ۂɓI�Ƃ̂�����Bullet�N���X���s�����̃N���X��Bullet�N���X�Ƒ��N���X�Ƃ̒���
 *		CmnGameAccesser��ʂ��Ă��̃N���X���Ǘ����Ă���Bullet�N���X�̈ʒu�����擾�����肷��
 *		
 */
/* ====================================================================== */

#ifndef __SYSTEM_ATTACK_GUN__
#define __SYSTEM_ATTACK_GUN__

#include "Common/CommonDefine.h"
#include "System/Task/SystemTaskUnit.h"
#include "Bullet.h"

// �Œ�l
static const uint32_t SHOOT_INTERBAL_DEFAULT	= 16;

class AttackGun : public TaskUnit
{

public:

	struct GunState{
		uint32_t	m_shootInterval;	// ���ˊԊu
		float		m_speed;			// ���˃X�s�[�h
		uint32_t	m_damage;			// �_���[�W

		void Init(){
			m_shootInterval	= SHOOT_INTERBAL_DEFAULT;
			m_speed			= 15.0f;
			m_damage		= 20;
		}
	};

	static AttackGun *CreateGun( const Common::OWNER_TYPE &ownerType ); 
	~AttackGun(void);

	// �e�̔���(�o�^)�ƍ폜
	void ShootBullet( math::Vector2 pos = DEFAULT_VECTOR2, math::Vector2 vec = math::Vector2( 0.0f, 1.0f ) );

	// �e�̃X�e�[�g�X�V
	GunState &UpdateGunState(){ return m_currState; }

protected:

	virtual void Update() override;
	virtual bool DieMain() override;

private:
	
	AttackGun( const Common::OWNER_TYPE &ownerType );

	Common::OWNER_TYPE		m_owner;
	GunState				m_currState;	// �e�̃X�e�[�^�X
	uint32_t				m_intervalTime; // ���˂̊Ԋu
	std::vector<Bullet*>	m_magazine;		// �e�̊Ǘ��x�N�^

};

#endif

