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

		// ���̗\�񂪂Ȃ���΃C���^�[�o����݂���
		if( !m_reserveInfo.m_isReserve ){
			m_intervalTime = m_currState.m_interval;
		}
	}
}

/* ================================================ */
/**
/ * @brief	�a������
/ */
/* ================================================ */
void AttackBlade::CreateSlashing( const math::Vector2 &pos, const math::Vector2 &vec, const Slashing::TYPE_SLASHING &type, const uint32_t &damage )
{
	CreateSlashingMain( pos, vec, type, ( damage == 0 ) ? m_currState.m_damage : damage );
}
void AttackBlade::CreateSlashingMain( const math::Vector2 &pos, const math::Vector2 &vec, const Slashing::TYPE_SLASHING &type, const uint32_t &damage )
{
	if( !m_currSlashing && m_intervalTime == 0 ){
		// TaskUnit�p���ŃA�j���I����Ɏ��E����̂Ő������邾��
		m_currSlashing = NEW Slashing( m_owner, type, pos, vec, damage, m_currState.m_deleteBullet );
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
/ * @brief	����Lv�̐ݒ�ƃX�e�[�^�X���f
/ */
/* ================================================ */
void AttackBlade::SetBladeLevel( const uint32_t &level )
{
	m_currState.m_bladeLv	= level;
	m_currState.m_damage	= SLASHING_DAMAGE_DEFAULT + Utility::ConvertLevelToBaseState( Common::BASE_STATE_BLADE_LEVEL, level );
	m_currState.m_interval	= 20;//ConvertLevelToBaseState( Common::BASE_STATE_BLADE_LEVEL, playData.m_playerBaseStateLv[Common::BASE_STATE_BLADE_LEVEL] );
	if( m_owner == Common::OWNER_PLAYER && m_currState.m_bladeLv == Common::STATUS_LEVEL_MAX ){
		// �v���C���[�̏����N���X�Ń��x�����ő�Ȃ�e������������悤�ɂ���
		m_currState.m_deleteBullet = true;
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

