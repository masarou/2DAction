/* ====================================================================== */
/**
 * @brief  �v���C���[�̍U���Ǘ��N���X
 *
 * @note
 *		Blade�N���X���x�N�^�Ŏ���
 *		���ۂɓI�Ƃ̂�����Blade�N���X���s�����̃N���X��Blade�N���X�Ƒ��N���X�Ƃ̒���
 *		CmnGameAccesser��ʂ��Ă��̃N���X���Ǘ����Ă���Blade�N���X�̈ʒu�����擾�����肷��
 *		
 */
/* ====================================================================== */

#ifndef __SYSTEM_ATTACK_GUN__
#define __SYSTEM_ATTACK_GUN__

#include "Common/CommonDefine.h"
#include "System/Task/SystemTaskUnit.h"


// �Œ�l
static const uint32_t SHOOT_TO_OVERHEAT_DEFAULT	= 12;
static const uint32_t SHOOT_SPEED_DEFAULT		= 20;
static const uint32_t SHOOT_DAMAGE_DEFAULT		= 25;
static const uint32_t SHOOT_INTERVAL_DEFAULT	= 20;

class Bullet;

class AttackGun : public TaskUnit
{

public:

	struct GunState{
		uint32_t	m_damageLv;			// �}�V���K�����x��
		uint32_t	m_intervalLv;		// �}�V���K�����x��
		uint32_t	m_timeToOverHeat;	// �I�[�o�[�q�[�g�܂ł̎���
		uint32_t	m_shootInterval;	// ���ˊԊu
		uint32_t	m_speed;			// ���˃X�s�[�h
		uint32_t	m_damage;			// �_���[�W

		void Init(){
			m_damageLv		= 0;
			m_intervalLv	= 0;
			m_timeToOverHeat= SHOOT_TO_OVERHEAT_DEFAULT;
			m_shootInterval	= SHOOT_INTERVAL_DEFAULT;
			m_speed			= SHOOT_SPEED_DEFAULT;
			m_damage		= SHOOT_DAMAGE_DEFAULT;
		}
	};

	static AttackGun *CreateGun( const Common::OWNER_TYPE &ownerType ); 
	~AttackGun(void);

	// �e�̔���(�o�^)�ƍ폜
	void ShootBullet( const math::Vector2 &pos, const math::Vector2 &vec );
	void ShootBullet( const math::Vector2 &pos, const math::Vector2 &vec, const uint32_t &damage, const uint32_t &speed );

	// �e�̃X�e�[�g�X�V
	GunState &UpdateGunState(){ return m_currState; }

	// �}�V���K����Lv�ݒ�ƃX�e�[�^�X���f
	void SetGunLevel( const uint32_t &damageLv, const uint32_t &speedLv );

protected:
	
	virtual void Update() override;
	virtual bool DieMain() override;

private:
	
	AttackGun( const Common::OWNER_TYPE &ownerType );

	Common::OWNER_TYPE		m_owner;
	bool					m_isOverHeat;
	uint32_t				m_overHeatTime;
	GunState				m_currState;	// �e�̃X�e�[�^�X
	uint32_t				m_intervalTime; // ���˂̊Ԋu(�ꔭ�P��)
	uint32_t				m_totalIntervalTime; // ���˂̊Ԋu(�g�[�^��)
	std::vector<Bullet*>	m_magazine;		// �e�̊Ǘ��x�N�^

};

#endif

