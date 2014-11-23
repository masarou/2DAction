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


AttackGun::AttackGun(void)
	: m_intervalTime( 0 )
{
	m_currState.Init();
}


AttackGun::~AttackGun(void)
{
}

bool AttackGun::DieMain()
{
	// 保持しているクラスをすべて解放
	std::vector<Bullet*>::iterator it = m_magazine.begin();
	while(m_magazine.empty() == 0){
		Bullet *bullet = (*it);
		it = m_magazine.erase(it);
		SAFE_DELETE(bullet);
	}
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
	for( uint32_t i = 0; i < m_magazine.size() ; ++i ){
		m_magazine.at( i )->Update();
	}

	// 生成されて一定時間を超えたものは削除
	for( uint32_t i = 0; i < m_magazine.size() ; ++i ){
		if( m_magazine.at( i )->GetLiveTime() >= BULLET_LIVE_TIME ){
			DeleteBullet( m_magazine.at( i )->GetUniqueNumber() );
		}
	}
}

void AttackGun::DrawUpdate()
{
	// 弾描画
	for( uint32_t i = 0; i < m_magazine.size() ; ++i ){
		m_magazine.at( i )->Draw();
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
	if( m_intervalTime == 0 ){
		static uint32_t uniqueNum = 0;
		Bullet *bul = NEW Bullet( uniqueNum, pos, vec, m_currState.m_speed );
		m_magazine.push_back( bul );
		
		// 発射音を鳴らす
		SoundManager::GetInstance()->PlaySE("ShootBullet");

		// 一定間隔の時間を設ける
		m_intervalTime += m_currState.m_shootInterval;
	}
}

/* ================================================ */
/**
 * @brief	弾の削除(画面外に出た、敵に当たった等々)
 */
/* ================================================ */
void AttackGun::DeleteBullet( uint32_t uniqueNumber )
{
	std::vector<Bullet*>::iterator it = m_magazine.begin();
	for( uint32_t i = 0; i < m_magazine.size() ; ++i ){
		if( (*it)->GetUniqueNumber() == uniqueNumber ){
			Bullet *pTmp = ( *it );
			m_magazine.erase( it );
			SAFE_DELETE( pTmp );
			break;
		}
		++it;
	}
}


