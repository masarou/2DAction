///* ====================================================================== */
///**
// * @brief  斬撃アニメ管理クラス
// *
// * @note
// *		斬撃を連続で出すときの時間管理、威力や種類を担う
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
		// アニメが終了して保持しているクラスが自殺するので次に備えてNULLセット
		m_currSlashing = NULL;
	}
}

/* ================================================ */
/**
/ * @brief	各種Update関数
/ */
/* ================================================ */
void AttackBlade::CreateSlashing( const math::Vector2 &pos, const math::Vector2 &vec, const Slashing::TYPE_SLASHING &type )
{
	if( !m_currSlashing && m_intervalTime == 0 ){
		// TaskUnit継承でアニメ終了後に自殺するので生成するだけ
		m_currSlashing = NEW Slashing( m_owner, type, pos, vec, m_currState.m_damage );
		SetChildUnit( m_currSlashing );

		// 斬撃効果音
		SoundManager::GetInstance()->PlaySE("Slashing1st");

		// 連続してできる攻撃の最後なら次の攻撃までに時間を設ける
		if( type == Slashing::TYPE_3RD ){
			m_intervalTime = m_currState.m_interval;
		}
	}
	else if( m_intervalTime == 0 ){
		// 斬撃予約をして生成できるようになり次第作る
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
/ * @brief	各種Update関数
/ */
/* ================================================ */
void AttackBlade::Update()
{
	if( m_intervalTime > 0 ){
		--m_intervalTime;
	}

	// 斬撃生成予約があれば次を作成
	if( !m_currSlashing && m_reserveInfo.m_isReserve && m_intervalTime == 0 ){
		CreateSlashing( m_reserveInfo.m_pos, m_reserveInfo.m_vec, m_reserveInfo.m_type );
		m_reserveInfo.Init();
	}
}

