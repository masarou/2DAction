///* ====================================================================== */
///**
// * @brief  �a���A�j���Ǘ��N���X
// *
// * @note
// *		�a����A���ŏo���Ƃ��̎��ԊǗ��A�З͂��ނ�S��
// *		
// */
///* ====================================================================== */

#ifndef __ATTACK_BLADE__
#define __ATTACK_BLADE__

#include "Common/CommonDefine.h"
#include "Game/Game2DBase.h"
#include "System/Task/SystemTaskUnit.h"
#include "Slashing.h"

// �Œ�l
static uint32_t SLASHING_INTERBAL_DEFAULT	= 20;	// �a���̐����Ԋu
static uint32_t SLASHING_DAMAGE_DEFAULT		= 35;	// �a���̈З�

class AttackBlade : public TaskUnit
{

public:

	struct BladeState{

		uint32_t	m_bladeLv;			// �a��Lv
		uint32_t	m_damage;			// �_���[�W
		uint32_t	m_interval;			// �a���Ԋu
		bool		m_deleteBullet;		// �e�����������邩

		void Init(){
			m_bladeLv		= 0;
			m_damage		= SLASHING_DAMAGE_DEFAULT;
			m_interval		= 0;
			m_deleteBullet	= false;
		}
	};

	static AttackBlade *CreateAttackBlade( const Common::OWNER_TYPE &ownerType );
	~AttackBlade(void);

	// �a������
	void CreateSlashing( const math::Vector2 &pos, const math::Vector2 &vec, const Slashing::TYPE_SLASHING &type = Slashing::TYPE_1ST, const uint32_t &damage = 0 );

	// ���̃X�e�[�^�X
	BladeState &UpdateBladeState(){ return m_currState; }

	// ����Lv�̐ݒ�ƃX�e�[�^�X���f
	void SetBladeLevel( const uint32_t &level );

	// ���ݍU����(�a���A�j�����o�Ă���)���ǂ���
	bool IsSlashingAnimPlay(){ return m_currSlashing ? true : false ;}

protected:
	
	virtual void Update() override;
	virtual bool DieMain() override;
	virtual void MessageReceive( const Message &msg );

private:

	struct RESERVE_SLASHING_INFO{
		bool					m_isReserve;
		Slashing::TYPE_SLASHING	m_type;
		math::Vector2			m_pos;
		math::Vector2			m_vec;
		void Init(){
			m_isReserve	= false;
			m_type		= Slashing::TYPE_MAX;
			m_pos		= math::Vector2();
			m_vec		= math::Vector2();
		}
	};
	
	// �a������
	void CreateSlashingMain( const math::Vector2 &pos, const math::Vector2 &vec, const Slashing::TYPE_SLASHING &type, const uint32_t &damage );

	AttackBlade( const Common::OWNER_TYPE &ownerType );
	
	Common::OWNER_TYPE		m_owner;
	BladeState				m_currState;		// ���̃X�e�[�^�X
	uint32_t				m_intervalTime;		// �a�������̊Ԋu
	RESERVE_SLASHING_INFO	m_reserveInfo;		// �a���\����

	Slashing				*m_currSlashing;	// �������̎a��
};

#endif
