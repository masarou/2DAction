/* ====================================================================== */
/**
 * @brief  敵管理クラス
 *
 * @note
 *		
 */
/* ====================================================================== */

#include "EnemyBase.h"
#include "EnemyManager.h"

#include "Common/CommonDefine.h"
#include "Common/Utility/CommonGameUtility.h"
#include "Game/GameRegister.h"
#include "Game/Effect/GameEffect.h"
#include "Game/GameScoreRecorder.h"
#include "System/Sound/SystemSoundManager.h"

EnemyBase::EnemyBase( const std::string &jsonName, const uint32_t &uniqueId, const Common::ENEMY_KIND &kind )
: m_enemyState( ENEMY_INIT )
, m_uniqueIdOfEnemyAll( uniqueId )
, m_enemyKind( kind )
, m_HP(10)
, m_eye(math::Vector2( 1.0f, 0.0f ))
, m_pEnemyAI( NULL )
, m_nextAI( Common::AI_NONE )
, m_prevAI( Common::AI_NONE )
{
	m_textureEnemy.Init();
	m_textureEnemy.m_pTex2D = NEW Game2DBase( jsonName.c_str() );
	m_textureEnemy.m_texInfo.m_fileName = jsonName;
	m_textureEnemy.m_pTex2D->SetDrawInfo( m_textureEnemy.m_texInfo );

	m_textureLife.Init();
	m_textureLife.m_pTex2D = NEW Game2DBase( "enemyGauge.json" );
	m_textureLife.m_texInfo.m_fileName = "enemyGauge.json";
	m_textureLife.m_pTex2D->SetDrawInfo( m_textureLife.m_texInfo );
}

EnemyBase::~EnemyBase()
{
	m_textureEnemy.DeleteAndInit();
	m_textureLife.DeleteAndInit();
	SAFE_DELETE( m_pEnemyAI );
}

bool EnemyBase::Init()
{
	m_HP		= GetEnemyDefaultHP();

	// 直前のAIがないので同じAIにしておく
	m_prevAI	= Common::AI_SEARCHING;
	m_nextAI	= Common::AI_SEARCHING;

	if( !m_pEnemyAI ){
		m_pEnemyAI = ChangeEnemyAI( m_nextAI );
	}

	return InitMain();
}

void EnemyBase::UpdateEnemy()
{
	if( m_pEnemyAI && m_nextAI != Common::AI_MAX )
	{
		// AI変更
		m_prevAI = m_pEnemyAI->GetAIKind();
		SAFE_DELETE( m_pEnemyAI );
		m_pEnemyAI = ChangeEnemyAI( m_nextAI );
		m_pEnemyAI->SetThingingEnemy(this);
		m_nextAI = Common::AI_MAX;
	}

	if( m_pEnemyAI ){
		m_pEnemyAI->Exec( m_textureEnemy.m_texInfo );
	}

	//// AIによって更新された値を反映
	m_textureEnemy.m_pTex2D->SetDrawInfo( m_textureEnemy.m_texInfo );

	// HP描画準備
	m_textureLife.m_texInfo.m_posOrigin.x = m_textureEnemy.m_texInfo.m_posOrigin.x;
	m_textureLife.m_texInfo.m_posOrigin.y = m_textureEnemy.m_texInfo.m_posOrigin.y + 30.0f;
	m_textureLife.m_texInfo.m_scale.x = ( m_HP/static_cast<float>(GetEnemyDefaultHP()) )*10.0f;
	m_textureLife.m_pTex2D->SetDrawInfo( m_textureLife.m_texInfo );
}

void EnemyBase::DrawEnemy()
{
	// AIがあるなら更新
	//if( m_pEnemyAI ){
		m_textureEnemy.m_pTex2D->DrawUpdate2D();
		m_textureLife.m_pTex2D->DrawUpdate2D();
	//}
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
		HitPlayreBullet( 30 ); // 仮
		break;

	}
}

/* ================================================ */
/**
 * @brief	描画情報取得
 */
/* ================================================ */
const TEX_DRAW_INFO &EnemyBase::GetDrawInfo() const
{
	return m_textureEnemy.m_texInfo;
}

/* ================================================ */
/**
 * @brief	イベントに対応した関数群
 */
/* ================================================ */

// プレイヤーの弾に当たった
void EnemyBase::HitPlayreBullet( uint32_t damageValue )
{
	if( m_HP <= damageValue ){
		m_HP = 0;
	}
	else{
		m_HP -= damageValue;
	}

	GameEffectDamage::GetInstance()->CreateEffectDamage( damageValue
		, static_cast<uint32_t>(m_textureEnemy.m_texInfo.m_posOrigin.x)
		, static_cast<uint32_t>(m_textureEnemy.m_texInfo.m_posOrigin.y));

	if( m_HP <= 0 ){
		// スコア追加
		ScoreRecorder::GetInstance()->ScoreEvent( ScoreRecorder::ENEMY_AAA_DEATH );

		// 爆破エフェクトを出す
		GameEffect *effect = NEW GameEffect( GameEffect::EFFECT_BOMB, static_cast<uint32_t>(m_textureEnemy.m_texInfo.m_posOrigin.x), static_cast<uint32_t>(m_textureEnemy.m_texInfo.m_posOrigin.y) );

		// 爆発SE鳴らす
		SoundManager::GetInstance()->PlaySE("Bomb");

		// managerに管理から外すように伝える
		EnemyManager *pEnemyMan = GameRegister::GetInstance()->UpdateManagerEnemy();
		pEnemyMan->DeleteEnemy( GetUniqueNumber() );
	}
}