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
#include "Slashing.h"
#include "Game/GameRegister.h"
#include "Common/Utility/CommonGameUtility.h"
#include "Game/Enemy/EnemyManager.h"
#include "System/Sound/SystemSoundManager.h"

// 固定値
static uint32_t SLASHING_INTERBAL = 10;	// 斬撃の生成間隔

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
void AttackBlade::CreateSlashing(  const math::Vector2 &pos, const math::Vector2 &vec )
{
	if( !m_currSlashing && m_intervalTime == 0 ){
		// TaskUnit継承でアニメ終了後に自殺するので生成するだけ
		m_currSlashing = NEW Slashing( m_owner, pos, vec, m_currState.m_damage );
		m_intervalTime = SLASHING_INTERBAL;
		SetChildUnit( m_currSlashing );

		// 斬撃効果音
		//SoundManager::GetInstance()->PlaySE("Slashing");
	}
	else{
		// 斬撃予約をして生成できるようになり次第作る
		m_reserveInfo.m_isReserve	= true;
		m_reserveInfo.m_pos			= pos;
		m_reserveInfo.m_vec			= vec;
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
		CreateSlashing( m_reserveInfo.m_pos, m_reserveInfo.m_vec );
		m_reserveInfo.Init();
	}
}

