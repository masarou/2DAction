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
#include "Game/GameRegister.h"

Bullet::Bullet( const uint32_t &uniqueNum, const math::Vector2 &pos, const math::Vector2 &vec, float speed )
	: m_uniqueNumber( uniqueNum )
	, m_liveTime( 0 )
	, m_bulletVec( vec )
	, m_speed( speed )
{
	m_textureBullet.Init();
	m_textureBullet.m_pTex2D = NEW Game2DBase("bullet.json");

	//!初期位置セット
	m_textureBullet.m_texInfo.Init();
	m_textureBullet.m_texInfo.m_fileName = "bullet.json";
	m_textureBullet.m_texInfo.m_pos = pos;
}

Bullet::~Bullet(void)
{
	m_textureBullet.DeleteAndInit();
}

void Bullet::Update()
{
	m_textureBullet.m_texInfo.m_pos += m_bulletVec * m_speed;
	m_textureBullet.m_pTex2D->SetDrawInfo(m_textureBullet.m_texInfo);

	// 敵に当たったかチェック
	bool isHit = GameRegister::GetInstance()->GetManagerEnemy()->CheckCollisionToBullet( this );
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
	m_textureBullet.m_pTex2D->DrawUpdate2D();
}

const TEX_DRAW_INFO &Bullet::GetDrawInfo()
{
	if( m_textureBullet.m_pTex2D == NULL ){
		DEBUG_ASSERT( 0, "弾の描画クラスがNULL");
	}
	return m_textureBullet.m_pTex2D->GetDrawInfo();
}