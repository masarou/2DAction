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
#include "Game/GameRecorder.h"
#include "System/Sound/SystemSoundManager.h"
#include "System/Draw2D/SystemDraw2DResource.h"

EnemyBase::EnemyBase( const std::string &jsonName, const uint32_t &uniqueId, const Common::ENEMY_KIND &kind, const uint32_t &enemyLevel, const math::Vector2 &enemyPos )
: TaskUnit("Enemy")
, Collision2DUnit( jsonName.c_str() )
, m_uniqueIdOfEnemyAll( uniqueId )
, m_enemyKind( kind )
, m_enemyLv( enemyLevel )
, m_HP(10)
, m_speed( 0 )
, m_eye(math::Vector2( 1.0f, 0.0f ))
, m_walkHeight( 0 )
, m_stunTime( 40 )
, m_pEnemyAI( NULL )
, m_nextAI( Common::AI_NONE )
, m_prevAI( Common::AI_NONE )
{
	if( m_enemyLv > 10 ){
		DEBUG_ASSERT( 0, "敵のレベルが想定外");
		m_enemyLv = 10;
	}

	m_actionInfoAI.Init();
	m_drawTexture.m_pTex2D->UpdateDrawInfo().m_fileName = jsonName;

	m_textureLife.Init();
	m_textureLife.m_pTex2D = Game2DBase::Create( "EnemyGauge.json" );
	m_textureLife.m_pTex2D->UpdateDrawInfo().m_fileName = "EnemyGauge.json";

	m_drawTexture.m_pTex2D->UpdateDrawInfo().m_posOrigin = enemyPos;
}

EnemyBase::~EnemyBase()
{
}

bool EnemyBase::Init()
{
	m_HP		= GetEnemyDefaultHP();
	m_speed		= GetEnemyDefaultSPD();

	// 直前のAIがないので同じAIにしておく
	m_prevAI	= GetEnemyDefaultAI();
	m_nextAI	= GetEnemyDefaultAI();

	if( !m_pEnemyAI ){
		m_pEnemyAI = Utility::CreateEnemyAI( m_nextAI );
		m_pEnemyAI->SetThingingEnemy(this);
	}

	// 初期位置セット
	if( m_drawTexture.m_pTex2D->UpdateDrawInfo().m_posOrigin == DEFAULT_VECTOR2 ){
		for(;;){
			math::Vector2 candidatePos = Utility::GetMapRandamPos( /*allowInWindow=*/false );
			// マップ上の動ける高さなら生成
			if( Utility::GetMapHeight( candidatePos ) <= GetWalkHeight() ){
				m_drawTexture.m_pTex2D->UpdateDrawInfo().m_posOrigin = candidatePos;
				DEBUG_PRINT( "敵生成 x = %f, y = %f\n", candidatePos.x, candidatePos.y );
				break;
			}
		}
	}

	return InitMain();
}

bool EnemyBase::DieMain()
{
	if( !DieMainCustom() ){
		return false;
	}

	// AIにも死亡通知
	if( m_pEnemyAI ){
		m_pEnemyAI->EnemyIsDead();
	}

	if( GameRegister::GetInstance()->GetManagerEnemy() ){
		GameRegister::GetInstance()->UpdateManagerEnemy()->RemoveEnemy( this );
	}
	
	m_textureLife.DeleteAndInit();
	SAFE_DELETE( m_pEnemyAI );

	if( GetEnemyHitPoint() == 0 ){
		// 倒された敵をカウントしておく
		GameRegister::GetInstance()->UpdateManagerGame()->AddDestroyCount();
	}

	return true;
};

/* ================================================ */
/**
 * @brief	各フェーズの更新関数
 */
/* ================================================ */
void EnemyBase::Update()
{
	if( m_stunTime == 0 ){
		if( m_pEnemyAI && m_nextAI != Common::AI_MAX )
		{
			// AI変更
			m_prevAI = m_pEnemyAI->GetAIKind();
			SAFE_DELETE( m_pEnemyAI );
			m_pEnemyAI = Utility::CreateEnemyAI( m_nextAI );
			m_pEnemyAI->SetThingingEnemy(this);
			m_nextAI = Common::AI_MAX;
		}

		if( m_pEnemyAI ){
			m_pEnemyAI->Exec( m_drawTexture.m_pTex2D->UpdateDrawInfo(), m_actionInfoAI );
		}

		// AIによって設定された行動を設定
		RefrectAIAction();
	}
	else{
		// スタン状態ならデクリメント
		--m_stunTime;
	}

	// HP描画準備
	Game2DBase *pTex2D = m_drawTexture.m_pTex2D;
	if( pTex2D ){
		TEX_DRAW_INFO drawInfo;
		const TEX_INIT_INFO &texInfo = TextureResourceManager::GetInstance()->GetLoadTextureInfo( pTex2D->GetDrawInfo().m_fileName.c_str() );
		drawInfo.m_posOrigin.x = pTex2D->GetDrawInfo().m_posOrigin.x - 5.0f;
		drawInfo.m_posOrigin.y = pTex2D->GetDrawInfo().m_posOrigin.y + (texInfo.m_sizeHeight / 2) + 10.0f;
		drawInfo.m_scale.x = ( m_HP/static_cast<float>(GetEnemyDefaultHP()) )*10.0f;
		m_textureLife.m_pTex2D->SetDrawInfo( drawInfo );
	}

	// HPがないなら死亡
	if( m_HP <= 0 ){
		TaskStartDie();
	}
	else{
		// 派生先更新処理
		UpdateCustom();
	}
}

void EnemyBase::DrawUpdate()
{
	m_drawTexture.m_pTex2D->DrawUpdate2D();
	m_textureLife.m_pTex2D->DrawUpdate2D();
	
	// 派生先描画処理
	DrawUpdateCustom();
}

/* ================================================ */
/**
 * @brief	他クラスからのイベント処理
 */
/* ================================================ */
void EnemyBase::EventUpdate( Common::CMN_EVENT &eventId )
{
	switch( eventId.m_event ){
	default:

		break;

	case Common::EVENT_HIT_BULLET_PLAYER:	// Playerの弾に当たった
		ReduceDamage( eventId );
		HitPlayreBullet( eventId.m_eventValue );
		break;

	case Common::EVENT_HIT_BLADE_PLAYER:	// Playerの斬撃に当たった
		ReduceDamage( eventId );
		HitPlayreSlashing( eventId.m_eventValue );
		break;
	}

	if( m_pEnemyAI )
	{
		// AIにもイベントがあったことを伝える
		m_pEnemyAI->EnemyRecievedEvent( eventId );
	}
}

/* ================================================ */
/**
 * @brief	描画情報取得
 */
/* ================================================ */
const TEX_DRAW_INFO &EnemyBase::GetDrawInfo() const
{
	if( !m_drawTexture.m_pTex2D ){
		DEBUG_ASSERT( 0, "必要なクラスが作成されていない");
	}
	return m_drawTexture.m_pTex2D->GetDrawInfo();
}

/* ================================================ */
/**
 * @brief	レベル取得
 */
/* ================================================ */
const uint32_t &EnemyBase::GetEnemyLevel() const
{
	return m_enemyLv;
}

/* ================================================ */
/**
 * @brief	現在のAI取得
 */
/* ================================================ */
const Common::ENEMY_AI EnemyBase::GetCurrentAIKind() const
{
	Common::ENEMY_AI ai = Common::AI_MAX;
	if( m_pEnemyAI ){
		ai = m_pEnemyAI->GetAIKind();
	}
	return ai;
}

/* ================================================ */
/**
 * @brief	イベントに対応した関数群
 */
/* ================================================ */

// プレイヤーの弾に当たった
void EnemyBase::HitPlayreBullet( const uint32_t &damageValue )
{
	UpdateEnemyDamage( damageValue );
}
// プレイヤーの斬撃に当たった
void EnemyBase::HitPlayreSlashing( const uint32_t &damageValue )
{
	SetStunTime( 10 );
	GameEffect::CreateEffect( GameEffect::EFFECT_SLASHING_HIT, m_drawTexture.m_pTex2D->GetDrawInfo().m_posOrigin );
	UpdateEnemyDamage( damageValue );
}

/* ================================================ */
/**
 * @brief	HPがなくなり倒されたときに呼ばれる
 */
/* ================================================ */
void EnemyBase::EnemyDeath()
{
	// 爆破エフェクトを出す
	GameEffect::CreateEffect( GameEffect::EFFECT_BOMB, m_drawTexture.m_pTex2D->GetDrawInfo().m_posOrigin );

	// 爆発SE鳴らす
	SoundManager::GetInstance()->PlaySE("Death");

	// 死亡
	TaskStartDie();
}

/* ================================================ */
/**
 * @brief	AI結果を反映
 */
/* ================================================ */
void EnemyBase::RefrectAIAction()
{
	// 格納されたイベントメッセージをセット
	for( uint32_t i = 0; i < m_actionInfoAI.m_pushEventArray.size(); ++i){
		Common::CMN_EVENT eventInfo;
		eventInfo.Init();
		eventInfo.m_event = m_actionInfoAI.m_pushEventArray.at(i).m_EventMessage;
		SystemMessageManager::GetInstance()->PushMessage( GetUniqueId(), eventInfo );
	}
	m_actionInfoAI.m_pushEventArray.clear();
}

/* ================================================ */
/**
 * @brief	ダメージ共通処理
 */
/* ================================================ */
void EnemyBase::UpdateEnemyDamage( const uint32_t &damageValue )
{
	if( m_HP <= damageValue ){
		m_HP = 0;
	}
	else{
		m_HP -= damageValue;
	}

	uint32_t totalDamage = damageValue;
	float rate = static_cast<float>(Utility::GetRandamValue( 120, 80 )) / 100.0f;
	totalDamage *= rate;

	//ダメージエフェクト作成
	GameEffectDamage::GetInstance()->CreateEffectDamage( totalDamage
		, static_cast<uint32_t>(m_drawTexture.m_pTex2D->GetDrawInfo().m_posOrigin.x)
		, static_cast<uint32_t>(m_drawTexture.m_pTex2D->GetDrawInfo().m_posOrigin.y));

	if( m_HP <= 0 ){
		// スコア追加
		Common::ENEMY_KIND kind = GetKind();
		switch( kind ){
		case Common::ENEMY_KIND_SLIME:
			GameRecorder::GetInstance()->ScoreEvent( GameRecorder::ENEMY_SLIME_DEATH, GetEnemyLevel() );
			break;
		case Common::ENEMY_KIND_SLIME_ANOTHER:
			GameRecorder::GetInstance()->ScoreEvent( GameRecorder::ENEMY_SLIME_ANOTHER_DEATH, GetEnemyLevel() );
			break;
		case Common::ENEMY_KIND_AHRIMAN:
			GameRecorder::GetInstance()->ScoreEvent( GameRecorder::ENEMY_AHRIMAN_DEATH, GetEnemyLevel() );
			break;
		case Common::ENEMY_KIND_COW:
			GameRecorder::GetInstance()->ScoreEvent( GameRecorder::ENEMY_COW_DEATH, GetEnemyLevel() );
			break;
		case Common::ENEMY_KIND_RANGER:
			GameRecorder::GetInstance()->ScoreEvent( GameRecorder::ENEMY_BOSS_DEATH, GetEnemyLevel() );
			break;
		case Common::ENEMY_KIND_SLIME_KING:
			GameRecorder::GetInstance()->ScoreEvent( GameRecorder::ENEMY_SLIME_KING_DEATH, GetEnemyLevel() );
			break;
		case Common::ENEMY_KIND_WIZARD:
			GameRecorder::GetInstance()->ScoreEvent( GameRecorder::ENEMY_WIZARD_DEATH, GetEnemyLevel() );
			break;
		case Common::ENEMY_KIND_DRAGON:
			GameRecorder::GetInstance()->ScoreEvent( GameRecorder::ENEMY_DRAGON_DEATH, GetEnemyLevel() );
			break;
		case Common::ENEMY_KIND_LAST_BOSS:
			GameRecorder::GetInstance()->ScoreEvent( GameRecorder::ENEMY_LAST_BOSS_DEATH, GetEnemyLevel() );
			break;
		}

		if( GetStatus() != TASK_PRE_DIE ){
			// 死亡
			EnemyDeath();
		}
	}

	if( damageValue != 0 ){
		// 連続Hit数加算
		GameRecorder::GetInstance()->IncHitCounter();
	}
}
