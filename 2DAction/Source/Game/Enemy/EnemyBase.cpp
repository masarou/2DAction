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

EnemyBase::EnemyBase( std::string jsonName, uint32_t uniqueId, Common::ENEMY_KIND kind )
: m_uniqueIdOfEnemyAll( uniqueId )
, m_enemyKind( kind )
{
	m_enemy2D = NEW Game2DBase( jsonName.c_str() );
	m_enemyInfo.Init();
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
		GameEffect *effect = NEW GameEffect( GameEffect::EFFECT_BOMB, 50,50);

		// managerに管理から外すように伝える
		EnemyManager::GetInstance()->DeleteEnemy( GetUniqueNumber() );
	}
}