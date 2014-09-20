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

Bullet::Bullet( uint32_t uniqueNum, math::Vector2 &pos, math::Vector2 &vec )
	: m_uniqueNumber( uniqueNum )
	, m_liveTime( 0 )
	, m_drawBullet( NULL )
	, m_bulletVec( vec )
	, m_speed( 12.0f )
{
	m_drawBullet = NEW Game2DBase("player.json");

	// 描画情報セット
	TEX_DRAW_INFO info;
	m_bulletInfo.Init();

	//!初期位置セット
	m_bulletInfo.m_pos = pos;
	m_drawBullet->SetDrawInfo(m_bulletInfo);
}

Bullet::~Bullet(void)
{
	SAFE_DELETE(m_drawBullet);
}

void Bullet::Update()
{
	m_bulletInfo.m_pos += m_bulletVec * m_speed;
	m_bulletInfo.m_offset = gameUtility::GetPlayerOffsetPos();
	m_drawBullet->SetDrawInfo(m_bulletInfo);

	++m_liveTime;
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