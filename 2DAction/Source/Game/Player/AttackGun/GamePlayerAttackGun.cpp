/* ====================================================================== */
/**
 * @brief  プレイヤーの攻撃管理クラス
 *
 * @note
 *		
 */
/* ====================================================================== */

#include "GamePlayerAttackGun.h"
#include "System/Sound/SystemSoundManager.h"

AttackGun *AttackGun::CreateGun( const Common::OWNER_TYPE &ownerType )
{
	return NEW AttackGun( ownerType );
}

AttackGun::AttackGun( const Common::OWNER_TYPE &ownerType )
: TaskUnit("AttackGun")
, m_intervalTime( 0 )
, m_owner( ownerType )
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
 * @brief	位置、描画更新関数
 */
/* ================================================ */
void AttackGun::Update()
{
	// 次の弾発射までの時間を減算
	if( m_intervalTime > 0){
		--m_intervalTime;
	}
}

/* ================================================ */
/**
 * @brief	弾の発射
 */
/* ================================================ */
void AttackGun::ShootBullet( math::Vector2 pos, math::Vector2 vec )
{
	if( m_intervalTime == 0 ){
		Bullet *bul = NEW Bullet( m_owner, pos, vec, m_currState.m_damage, m_currState.m_speed );
		m_magazine.push_back( bul );
		
		// 発射音を鳴らす
		SoundManager::GetInstance()->PlaySE("ShootBullet");

		// 一定間隔の時間を設ける
		m_intervalTime += m_currState.m_shootInterval;
	}
}