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
	std::string jsonStr = ( m_ownerType == Common::OWNER_PLAYER ) ? "Bullet.json" : "BulletEnemy.json" ;
	m_drawTexture.m_pTex2D = Game2DBase::Create( jsonStr.c_str() );

	//!初期位置セット
	TEX_DRAW_INFO drawInfo;
	drawInfo.Init();
	drawInfo.m_fileName = jsonStr;
	drawInfo.m_posOrigin = pos;
	m_drawTexture.m_pTex2D->SetDrawInfo( drawInfo );

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
	m_drawTexture.m_pTex2D->UpdateDrawInfo().m_posOrigin += m_bulletVec * static_cast<float>(m_speed);
	++m_liveTime;

	if( m_liveTime >= m_liveTimeMax ){
		// 生成から一定時間たったなら自殺
		TaskStartDie();
	}

	// 厳密に確認する必要ないのでマップの高さだけを見る
	if( m_ownerType == Common::OWNER_PLAYER
		&& Utility::GetMapHeight( m_drawTexture.m_pTex2D->UpdateDrawInfo().m_posOrigin ) > 0 ){
		// 壁などに当たったので自殺
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
void Bullet::EventUpdate( Common::CMN_EVENT &eventId )
{
	switch( eventId.m_event ){
	default:

		break;

	case Common::EVENT_HIT_ENEMY_SLIME:	// 敵に当たった
	case Common::EVENT_HIT_ENEMY_SLIME_ANOTHER:
	case Common::EVENT_HIT_ENEMY_AHRIMAN:
	case Common::EVENT_HIT_ENEMY_COW:
	case Common::EVENT_HIT_ENEMY_BOSS:
	case Common::EVENT_HIT_ENEMY_SLIME_KING:
	case Common::EVENT_HIT_ENEMY_WIZARD:
	case Common::EVENT_HIT_WIZARD_CRYSTAL:
	case Common::EVENT_HIT_DRAGON:
	case Common::EVENT_HIT_BOSS:
	case Common::EVENT_HIT_BOSS_LEFT:
	case Common::EVENT_HIT_BOSS_RIGHT:
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

	case Common::EVENT_HIT_BLADE_PLAYER:
		// 敵の弾がプレイヤーの斬撃に当たったらかき消される
		if( m_ownerType == Common::OWNER_ENEMY ){
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
