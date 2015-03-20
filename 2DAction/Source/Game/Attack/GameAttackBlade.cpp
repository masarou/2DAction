///* ====================================================================== */
///**
// * @brief  �a���A�j���Ǘ��N���X
// *
// * @note
// *		�a����A���ŏo���Ƃ��̎��ԊǗ��A�З͂��ނ�S��
// *		
// */
///* ====================================================================== */

#include "GameAttackBlade.h"
#include "Slashing.h"
#include "Game/GameRegister.h"
#include "Common/Utility/CommonGameUtility.h"
#include "Game/Enemy/EnemyManager.h"
#include "System/Sound/SystemSoundManager.h"

// �Œ�l
static uint32_t SLASHING_INTERBAL = 10;	// �a���̐����Ԋu

AttackBlade *AttackBlade::CreateAttackBlade( const Common::OWNER_TYPE &ownerType )
{
	return NEW AttackBlade( ownerType );
}

AttackBlade::AttackBlade( const Common::OWNER_TYPE &ownerType )
: TaskUnit( "AttackBlade" )
, m_currSlashing( NULL )
, m_owner( ownerType )
, m_intervalTime( 0 )
{
	m_currState.Init();
	m_reserveInfo.Init();
}

AttackBlade::~AttackBlade(void)
{
}

bool AttackBlade::DieMain()
{
	return true;
}

void AttackBlade::MessageReceive( const Message &msg )
{
	if( msg.getSignal() == Slashing::SLASHING_ANIM_END ){
		// �A�j�����I�����ĕێ����Ă���N���X�����E����̂Ŏ��ɔ�����NULL�Z�b�g
		m_currSlashing = NULL;
	}
}

/* ================================================ */
/**
/ * @brief	�e��Update�֐�
/ */
/* ================================================ */
void AttackBlade::CreateSlashing(  const math::Vector2 &pos, const math::Vector2 &vec )
{
	if( !m_currSlashing && m_intervalTime == 0 ){
		// TaskUnit�p���ŃA�j���I����Ɏ��E����̂Ő������邾��
		m_currSlashing = NEW Slashing( m_owner, pos, vec, m_currState.m_damage );
		m_intervalTime = SLASHING_INTERBAL;
		SetChildUnit( m_currSlashing );

		// �a�����ʉ�
		//SoundManager::GetInstance()->PlaySE("Slashing");
	}
	else{
		// �a���\������Đ����ł���悤�ɂȂ莟����
		m_reserveInfo.m_isReserve	= true;
		m_reserveInfo.m_pos			= pos;
		m_reserveInfo.m_vec			= vec;
	}
}

/* ================================================ */
/**
/ * @brief	�e��Update�֐�
/ */
/* ================================================ */
void AttackBlade::Update()
{
	if( m_intervalTime > 0 ){
		--m_intervalTime;
	}

	// �a�������\�񂪂���Ύ����쐬
	if( !m_currSlashing && m_reserveInfo.m_isReserve && m_intervalTime == 0 ){
		CreateSlashing( m_reserveInfo.m_pos, m_reserveInfo.m_vec );
		m_reserveInfo.Init();
	}
}

