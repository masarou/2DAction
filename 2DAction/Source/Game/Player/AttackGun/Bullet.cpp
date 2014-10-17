/* ====================================================================== */
/**
 * @brief  GamePlayerAttackGunから発射される球クラス
 *
 * @note
 *		GamePlayerAttackGunを親として発射された球のクラス
 *		一発の玉の情報を持ち、スピードや方向、位置情報を持つ
 *		
 */
/* ====================================================================== */

#include "Bullet.h"
#include "Common/Utility/CommonGameUtility.h"
#include "Game/Enemy/EnemyManager.h"

Bullet::Bullet( const uint32_t &uniqueNum, const math::Vector2 &pos, const math::Vector2 &vec )
	: m_uniqueNumber( uniqueNum )
	, m_liveTime( 0 )
	, m_drawBullet( NULL )
	, m_bulletVec( vec )
	, m_speed( 12.0f )
{
	m_drawBullet = NEW Game2DBase("bullet.json");

	//!初期位置セット
	m_bulletInfo.Init();
	m_bulletInfo.m_fileName = "bullet.json";
	m_bulletInfo.m_pos = pos;
}

Bullet::~Bullet(void)
{
	SAFE_DELETE(m_drawBullet);
}

void Bullet::Update()
{
	m_bulletInfo.m_pos += m_bulletVec * m_speed;
	m_drawBullet->SetDrawInfo(m_bulletInfo);

	// 敵に当たったかチェック
	bool isHit = EnemyManager::GetInstance()->CheckCollisionToBullet( this );
	if( isHit ){
		m_liveTime = BULLET_LIVE_TIME;
	}
	else{
		++m_liveTime;
	}
}

void Bullet::Draw()
{
	// 弾描画
	m_drawBullet->DrawUpdate2D();
}

const TEX_DRAW_INFO &Bullet::GetDrawInfo()
{
	return m_drawBullet->GetDrawInfo();
}