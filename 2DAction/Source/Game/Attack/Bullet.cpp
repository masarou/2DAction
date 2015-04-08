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
#include "Game/GameRegister.h"
#include "Common/Utility/CommonGameUtility.h"
#include "Game/Enemy/EnemyManager.h"
#include "Game/Attack/GameAttackGun.h"

Bullet::Bullet( const Common::OWNER_TYPE ownerType, const math::Vector2 &pos, const math::Vector2 &vec, const uint32_t &damage, const uint32_t &speed )
: TaskUnit( "Bullet" )
, m_ownerType( ownerType )
, m_liveTime( 0 )
, m_liveTimeMax( 0 )
, m_bulletDamage( damage )
, m_bulletVec( vec )
, m_speed( speed )
{
	std::string jsonStr = ( m_ownerType == Common::OWNER_PLAYER ) ? "bullet.json" : "bulletEnemy.json" ;
	m_drawTexture.m_pTex2D = NEW Game2DBase( jsonStr.c_str() );

	//!初期位置セット
	m_drawTexture.m_texInfo.Init();
	m_drawTexture.m_texInfo.m_fileName = jsonStr;
	m_drawTexture.m_texInfo.m_posOrigin = pos;
	m_drawTexture.m_pTex2D->SetDrawInfo(m_drawTexture.m_texInfo);

	if( m_ownerType == Common::OWNER_PLAYER ){
		m_liveTimeMax = BULLET_LIVE_TIME_PLAYER;
	}
	else{
		m_liveTimeMax = BULLET_LIVE_TIME_ENEMY;
	}

}

Bullet::~Bullet(void)
{
}

bool Bullet::DieMain()
{
	return true;
}

bool Bullet::Init()
{
	return true;
}

const TEX_DRAW_INFO &Bullet::GetDrawInfo() const
{
	if( m_drawTexture.m_pTex2D == NULL ){
		DEBUG_ASSERT( 0, "弾の描画クラスがNULL");
	}
	return m_drawTexture.m_pTex2D->GetDrawInfo();
}

/* ================================================ */
/**
 * @brief	各種Update関数
 */
/* ================================================ */
void Bullet::Update()
{
	m_drawTexture.m_texInfo.m_posOrigin += m_bulletVec * static_cast<float>(m_speed);
	m_drawTexture.m_pTex2D->SetDrawInfo(m_drawTexture.m_texInfo);
	++m_liveTime;

	if( m_liveTime >= m_liveTimeMax ){
		// 生成から一定時間たったなら自殺
		TaskStartDie();
	}
}

void Bullet::DrawUpdate()
{
	// 弾描画
	m_drawTexture.m_pTex2D->DrawUpdate2D();
}

/* ================================================ */
/**
 * @brief	他クラスからのイベント処理
 */
/* ================================================ */
void Bullet::EventUpdate( const Common::CMN_EVENT &eventId )
{
	switch( eventId.m_event ){
	default:

		break;

	case Common::EVENT_HIT_ENEMY_AAA:	// 敵に当たった
	case Common::EVENT_HIT_ENEMY_BBB:
	case Common::EVENT_HIT_ENEMY_CCC:
		if( m_ownerType == Common::OWNER_PLAYER ){
			m_liveTime = m_liveTimeMax;
			TaskStartDie();
		}
		break;
		
	case Common::EVENT_HIT_PLAYER:
		if( m_ownerType == Common::OWNER_ENEMY ){
			m_liveTime = m_liveTimeMax;
			TaskStartDie();
		}
		break;
	}
}

/* ================================================ */
/**
 * @brief	このクラスの種類セット
 */
/* ================================================ */
const Common::TYPE_OBJECT Bullet::GetTypeObject() const
{
	Common::TYPE_OBJECT type = Common::TYPE_BULLET_PLAYER;
	if( m_ownerType == Common::OWNER_ENEMY ){
		type = Common::TYPE_BULLET_ENEMY;
	}
	return type;
}
