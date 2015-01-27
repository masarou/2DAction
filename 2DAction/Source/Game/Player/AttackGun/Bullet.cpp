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

Bullet::Bullet( const Common::OWNER_TYPE ownerType, const uint32_t &uniqueNum, const math::Vector2 &pos, const math::Vector2 &vec, float speed )
: TaskUnit( "Bullet" )
, Collision2DUnit( "bullet.json" )
, m_ownerType( ownerType )
, m_uniqueNumber( uniqueNum )
, m_liveTime( 0 )
, m_bulletDamage( 10 )
, m_bulletVec( vec )
, m_speed( speed )
{
	//!初期位置セット
	m_drawTexture.m_texInfo.Init();
	m_drawTexture.m_texInfo.m_fileName = "bullet.json";
	m_drawTexture.m_texInfo.m_posOrigin = pos;
	m_drawTexture.m_pTex2D->SetDrawInfo(m_drawTexture.m_texInfo);
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
	m_drawTexture.m_texInfo.m_posOrigin += m_bulletVec * m_speed;
	m_drawTexture.m_pTex2D->SetDrawInfo(m_drawTexture.m_texInfo);
	++m_liveTime;
}

void Bullet::CollisionUpdate()
{
	if( m_ownerType == Common::OWNER_PLAYER ){
		// 敵に当たったかチェック
		EnemyManager *pEnemyMan = GameRegister::GetInstance()->UpdateManagerEnemy();
		//bool isHit = pEnemyMan->CheckCollisionToBullet( this );
		//if( isHit ){
			m_liveTime = BULLET_LIVE_TIME;
		//}
		//else{
			
		//}
	}
	else{
		// プレイヤーに当たったかチェック

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
			m_liveTime = BULLET_LIVE_TIME;
			TaskStartDie();
		}
		break;
		
	case Common::EVENT_HIT_PLAYER:
		if( m_ownerType == Common::OWNER_ENEMY ){
			m_liveTime = BULLET_LIVE_TIME;
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
