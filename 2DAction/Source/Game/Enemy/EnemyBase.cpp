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

EnemyBase::EnemyBase( std::string jsonName, uint32_t uniqueId, Common::ENEMY_KIND kind )
: m_uniqueIdOfEnemyAll( uniqueId )
, m_enemyKind( kind )
{
	m_enemy2D = NEW Game2DBase( jsonName.c_str() );
	m_enemyInfo.Init();
}

void EnemyBase::DrawUpdate()
{
	m_enemy2D->DrawUpdate2D();
}

bool EnemyBase::DieMain()
{
	SAFE_DELETE(m_enemy2D);
	return true;
}

EnemyBase::~EnemyBase(void)
{
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
 * @brief	イベントに対応した関数群
 */
/* ================================================ */

// プレイヤーの弾に当たった
void EnemyBase::HitPlayreBullet()
{
	// もう死ぬ準備に入っているなら何もしない
	if(IsDie()){
		return;
	}

	--m_HP;
	if( m_HP <= 0 ){
		GameEffect *effect = NEW GameEffect( GameEffect::EFFECT_BOMB, m_,50);
		TaskStartDie();
	}
}