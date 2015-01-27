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
	// 生成されて一定時間を超えたものは削除
	for( auto it = m_magazine.begin(); it != m_magazine.end() ;){
		if( (*it)->GetLiveTime() >= BULLET_LIVE_TIME ){
			it = m_magazine.erase(it);
		}
		else{
			++it;
		}
	}
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
	static uint32_t uniqueNum = 0;
	if( m_intervalTime == 0 ){
		Bullet *bul = NEW Bullet( m_owner, uniqueNum, pos, vec, m_currState.m_speed );
		m_magazine.push_back( bul );
		
		// 発射音を鳴らす
		SoundManager::GetInstance()->PlaySE("ShootBullet");

		// 一定間隔の時間を設ける
		m_intervalTime += m_currState.m_shootInterval;
	}
	++uniqueNum;
}

/* ================================================ */
/**
 * @brief	弾の削除(画面外に出た、敵に当たった等々)
 */
/* ================================================ */
std::vector<Bullet*>::iterator AttackGun::DeleteBullet( uint32_t uniqueNumber )
{
	auto it = m_magazine.begin();
	for(; it != m_magazine.end() ; ++it ){
		if( (*it)->GetUniqueNumber() == uniqueNumber ){
			Bullet *pTmp = ( *it );
			it = m_magazine.erase( it );
			SAFE_DELETE( pTmp );
			break;
		}
	}
	return it;
}


