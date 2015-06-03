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
#include "Game/GameRegister.h"
#include "Common/Utility/CommonGameUtility.h"
#include "Game/Enemy/EnemyManager.h"
#include "System/Sound/SystemSoundManager.h"

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
void AttackBlade::CreateSlashing( const math::Vector2 &pos, const math::Vector2 &vec, const Slashing::TYPE_SLASHING &type )
{
	if( !m_currSlashing && m_intervalTime == 0 ){
		// TaskUnit�p���ŃA�j���I����Ɏ��E����̂Ő������邾��
		m_currSlashing = NEW Slashing( m_owner, type, pos, vec, m_currState.m_damage );
		SetChildUnit( m_currSlashing );

		// �a�����ʉ�
		SoundManager::GetInstance()->PlaySE("Slashing1st");

		// �A�����Ăł���U���̍Ō�Ȃ玟�̍U���܂łɎ��Ԃ�݂���
		if( type == Slashing::TYPE_3RD ){
			m_intervalTime = m_currState.m_interval;
		}
	}
	else if( m_intervalTime == 0 ){
		// �a���\������Đ����ł���悤�ɂȂ莟����
		m_reserveInfo.m_isReserve	= true;
		m_reserveInfo.m_pos			= pos;
		m_reserveInfo.m_vec			= vec;
		switch( m_currSlashing->GetTypeSlashing() ){
		case Slashing::TYPE_1ST:
			m_reserveInfo.m_type = Slashing::TYPE_2ND;
			break;
		case Slashing::TYPE_2ND:
			m_reserveInfo.m_type = Slashing::TYPE_3RD;
			break;
		case Slashing::TYPE_3RD:
			m_reserveInfo.Init();
			break;
		}
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
		CreateSlashing( m_reserveInfo.m_pos, m_reserveInfo.m_vec, m_reserveInfo.m_type );
		m_reserveInfo.Init();
	}
}

