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

class Slashing;

class AttackBlade : public TaskUnit
{

public:

	struct BladeState{

		uint32_t	m_damage;			// �_���[�W

		void Init(){
			m_damage	= 20;
		}
	};

	static AttackBlade *CreateAttackBlade( const Common::OWNER_TYPE &ownerType );
	~AttackBlade(void);

	// �a������
	void CreateSlashing(  const math::Vector2 &pos, const math::Vector2 &vec );

	// ���̃X�e�[�^�X
	BladeState &UpdateBladeState(){ return m_currState; }

	// ���ݍU����(�a���A�j�����o�Ă���)���ǂ���
	bool IsSlashingAnimPlay(){ return m_currSlashing ? true : false ;}

protected:
	
	virtual void Update() override;
	virtual bool DieMain() override;
	virtual void MessageReceive( const Message &msg );

private:

	struct RESERVE_SLASHING_INFO{
		bool			m_isReserve;
		math::Vector2	m_pos;
		math::Vector2	m_vec;
		void Init(){
			m_isReserve = false;
			m_pos = math::Vector2();
			m_vec = math::Vector2();
		}
	};

	AttackBlade( const Common::OWNER_TYPE &ownerType );
	
	Common::OWNER_TYPE		m_owner;
	BladeState				m_currState;		// ���̃X�e�[�^�X
	uint32_t				m_intervalTime;		// �a�������̊Ԋu
	RESERVE_SLASHING_INFO	m_reserveInfo;		// �a���\����

	Slashing				*m_currSlashing;	// �������̎a��
};

#endif
