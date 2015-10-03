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

		// 次の予約がなければインターバルを設ける
		if( !m_reserveInfo.m_isReserve ){
			m_intervalTime = m_currState.m_interval;
		}
	}
}

/* ================================================ */
/**
/ * @brief	斬撃生成
/ */
/* ================================================ */
void AttackBlade::CreateSlashing( const math::Vector2 &pos, const math::Vector2 &vec, const Slashing::TYPE_SLASHING &type, const uint32_t &damage )
{
	CreateSlashingMain( pos, vec, type, ( damage == 0 ) ? m_currState.m_damage : damage );
}
void AttackBlade::CreateSlashingMain( const math::Vector2 &pos, const math::Vector2 &vec, const Slashing::TYPE_SLASHING &type, const uint32_t &damage )
{
	if( !m_currSlashing && m_intervalTime == 0 ){
		// TaskUnit継承でアニメ終了後に自殺するので生成するだけ
		m_currSlashing = NEW Slashing( m_owner, type, pos, vec, damage, m_currState.m_deleteBullet );
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
/ * @brief	剣のLvの設定とステータス反映
/ */
/* ================================================ */
void AttackBlade::SetBladeLevel( const uint32_t &level )
{
	m_currState.m_bladeLv	= level;
	m_currState.m_damage	= SLASHING_DAMAGE_DEFAULT + Utility::ConvertLevelToBaseState( Common::BASE_STATE_BLADE_LEVEL, level );
	m_currState.m_interval	= 20;//ConvertLevelToBaseState( Common::BASE_STATE_BLADE_LEVEL, playData.m_playerBaseStateLv[Common::BASE_STATE_BLADE_LEVEL] );
	if( m_owner == Common::OWNER_PLAYER && m_currState.m_bladeLv == Common::STATUS_LEVEL_MAX ){
		// プレイヤーの所持クラスでレベルが最大なら弾をかき消せるようにする
		m_currState.m_deleteBullet = true;
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

