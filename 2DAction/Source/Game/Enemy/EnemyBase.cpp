/* ====================================================================== */
/**
 * @brief  敵管理クラス
 *
 * @note
 *		
 */
/* ====================================================================== */

#include "EnemyBase.h"
#include "Game/Effect/GameEffect.h"
#include "EnemyManager.h"

EnemyBase::EnemyBase( const std::string &jsonName, const uint32_t &uniqueId, const Common::ENEMY_KIND &kind )
: m_uniqueIdOfEnemyAll( uniqueId )
, m_enemyKind( kind )
, m_HP(1)
, m_eye(math::Vector2())
, m_enemy2D( NULL ) 
{
	m_enemy2D = NEW Game2DBase( jsonName.c_str() );

	//!初期情報セット
	m_enemyInfo.Init();
	m_enemyInfo.m_fileName = jsonName;
}

EnemyBase::~EnemyBase()
{
	SAFE_DELETE(m_enemy2D);
}

void EnemyBase::DrawEnemy()
{
	m_enemy2D->DrawUpdate2D();
}

/* ================================================ */
/**
 * @brief	他クラスからのイベント処理
 */
/* ================================================ */
void EnemyBase::EventUpdate( const Common::CMN_EVENT &eventId )
{
	switch( eventId.m_event ){
	default:

		break;

	case Common::EVENT_HIT_BULLET:	// Playerの弾に当たった
		HitPlayreBullet();
		break;

	}
}

/* ================================================ */
/**
 * @brief	描画情報取得
 */
/* ================================================ */
const TEX_DRAW_INFO &EnemyBase::GetDrawInfo()
{
	return m_enemyInfo;
}

/* ================================================ */
/**
 * @brief	イベントに対応した関数群
 */
/* ================================================ */

// プレイヤーの弾に当たった
void EnemyBase::HitPlayreBullet()
{
	--m_HP;

	if( m_HP <= 0 ){
		// 爆破エフェクトを出す
		GameEffect *effect = NEW GameEffect( GameEffect::EFFECT_BOMB, m_enemyInfo.m_pos.x, m_enemyInfo.m_pos.y);

		// managerに管理から外すように伝える
		EnemyManager::GetInstance()->DeleteEnemy( GetUniqueNumber() );
	}
}