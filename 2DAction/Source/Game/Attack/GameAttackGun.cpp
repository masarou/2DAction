/* ====================================================================== */
/**
 * @brief  プレイヤーの攻撃管理クラス
 *
 * @note
 *		
 */
/* ====================================================================== */

#include "GameAttackGun.h"
#include "Bullet.h"
#include "System/Sound/SystemSoundManager.h"
#include "Common/Utility/CommonGameUtility.h"

AttackGun *AttackGun::CreateGun( const Common::OWNER_TYPE &ownerType )
{
	return NEW AttackGun( ownerType );
}

AttackGun::AttackGun( const Common::OWNER_TYPE &ownerType )
: TaskUnit("AttackGun")
, m_owner( ownerType )
, m_isOverHeat( false )
, m_overHeatTime( 0 )
, m_intervalTime( 0 )
, m_totalIntervalTime( 0 )
{
	m_currState.Init();
}


AttackGun::~AttackGun(void)
{
}

bool AttackGun::DieMain()
{
	m_magazine.clear();
	return true;
}

/* ================================================ */
/**
 * @brief	マシンガンのLv設定とステータス反映
 */
/* ================================================ */
void AttackGun::SetGunLevel( const uint32_t &damageLv, const uint32_t &speedLv )
{
	if( damageLv > Common::STATUS_LEVEL_MAX || speedLv > Common::STATUS_LEVEL_MAX ){
		DEBUG_ASSERT( 0, "指定レベルが想定外" );
		return;
	}

	m_currState.m_damage			= damageLv;
	m_currState.m_intervalLv		= speedLv;
	m_currState.m_damage			= SHOOT_DAMAGE_DEFAULT + Utility::ConvertLevelToBaseState( Common::BASE_STATE_BULLET_DMG, damageLv );
	m_currState.m_timeToOverHeat	= SHOOT_TO_OVERHEAT_DEFAULT + Utility::ConvertLevelToBaseState( Common::BASE_STATE_BULLET_SPD, speedLv );
	switch( speedLv ){
	case 0:
	case 1:
	case 2:
	case 3:
		m_currState.m_shootInterval		= 5;
		break;
	case 4:
	case 5:
	case 6:
	case 7:
		m_currState.m_shootInterval		= 4;
		break;
	case 8:
	case 9:
		m_currState.m_shootInterval		= 3;
		break;
	}
	
	//m_speed				+= playData.m_playerBaseStateLv[Common::BASE_STATE_BULLET_DMG];
}

/* ================================================ */
/**
 * @brief	位置、描画更新関数
 */
/* ================================================ */
void AttackGun::Update()
{
	// 次の弾発射までの時間を減算
	if( m_intervalTime > 0){
		--m_intervalTime;
	}
	if( m_totalIntervalTime > 0){
		--m_totalIntervalTime;
	}

	if( m_overHeatTime > 0 ){
		--m_overHeatTime;
		if( m_overHeatTime == 0 ){
			m_isOverHeat = false;
		}
	}
}

/* ================================================ */
/**
 * @brief	弾の発射
 */
/* ================================================ */
void AttackGun::ShootBullet( const math::Vector2 &pos, const math::Vector2 &vec )
{
	ShootBullet( pos, vec, m_currState.m_damage, m_currState.m_speed );
}
void AttackGun::ShootBullet( const math::Vector2 &pos, const math::Vector2 &vec, const uint32_t &damage, const uint32_t &speed )
{
	if( m_owner == Common::OWNER_ENEMY
		|| m_intervalTime <= 30 && m_intervalTime == 0 && !m_isOverHeat ){
		Bullet *bul = NEW Bullet( m_owner, pos, vec, damage, speed );
		m_magazine.push_back( bul );
		
		// 発射音を鳴らす
		SoundManager::GetInstance()->PlaySE("ShootBullet", 9000);

		// 一定間隔の時間を設ける
		m_intervalTime += m_currState.m_shootInterval;
		m_totalIntervalTime += m_currState.m_shootInterval + 2;
	}

	if( m_totalIntervalTime > m_currState.m_timeToOverHeat ){
		m_totalIntervalTime = 0;
		m_overHeatTime = 60;
		m_isOverHeat = true;
	}
}