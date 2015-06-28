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

EnemyBase::EnemyBase( const std::string &jsonName, const uint32_t &uniqueId, const Common::ENEMY_KIND &kind, const uint32_t &enemyLevel )
: TaskUnit("Enemy")
, Collision2DUnit( jsonName.c_str() )
, m_uniqueIdOfEnemyAll( uniqueId )
, m_enemyKind( kind )
, m_enemyLv( enemyLevel )
, m_HP(10)
, m_speed( 0 )
, m_eye(math::Vector2( 1.0f, 0.0f ))
, m_walkHeight( 0 )
, m_stunTime( 0 )
, m_pEnemyAI( NULL )
, m_nextAI( Common::AI_NONE )
, m_prevAI( Common::AI_NONE )
{
	m_actionInfoAI.Init();
	m_drawTexture.m_pTex2D->UpdateDrawInfo().m_fileName = jsonName;

	m_textureLife.Init();
	m_textureLife.m_pTex2D = Game2DBase::Create( "EnemyGauge.json" );
	m_textureLife.m_pTex2D->UpdateDrawInfo().m_fileName = "EnemyGauge.json";

	//// 敵の強さを求める
	//uint32_t enemyLv = 0;
	//switch( GameRecorder::GetInstance()->GetGameStateOfProgress() ){
	//default:
	//	DEBUG_ASSERT( 0, "想定外のゲームの状態" );
	//	/* fall-through */
	//case GameRecorder::STATE_STAGE01:
	//case GameRecorder::STATE_STAGE02:
	//case GameRecorder::STATE_STAGE03:
	//case GameRecorder::STATE_STAGE04:
	//	m_enemyLv = 1;
	//	break;
	//case GameRecorder::STATE_STAGE05:
	//case GameRecorder::STATE_STAGE06:
	//case GameRecorder::STATE_STAGE07:
	//case GameRecorder::STATE_STAGE08:
	//	m_enemyLv = 2;
	//	break;
	//case GameRecorder::STATE_STAGE09:
	//case GameRecorder::STATE_STAGE10:
	//case GameRecorder::STATE_STAGE11:
	//case GameRecorder::STATE_STAGE12:
	//	m_enemyLv = 3;
	//	break;
	//}
}

EnemyBase::~EnemyBase()
{
}

bool EnemyBase::Init()
{
	m_HP		= GetEnemyDefaultHP();
	m_speed		= GetEnemyDefaultSPD();

	// 直前のAIがないので同じAIにしておく
	m_prevAI	= Common::AI_SEARCHING;
	m_nextAI	= Common::AI_SEARCHING;

	// ボスなら専用AIへ
	if( GetKind() == Common::ENEMY_KIND_BOSS ){
		m_prevAI	= Common::AI_ATTACK_NEAR;
		m_nextAI	= Common::AI_ATTACK_NEAR;
	}
	// スライムキングAIへ
	else if( GetKind() == Common::ENEMY_KIND_SLIME_KING ){
		m_prevAI	= Common::AI_ATTACK_NEAR;
		m_nextAI	= Common::AI_ATTACK_NEAR;
	}

	if( !m_pEnemyAI ){
		m_pEnemyAI = Utility::CreateEnemyAI( m_nextAI );
	}

	// 初期位置セット
	for(;;){
		math::Vector2 candidatePos = Utility::GetMapRandamPos( /*allowInWindow=*/false );
		// マップ上の動ける高さなら生成
		if( Utility::GetMapHeight( candidatePos ) <= GetWalkHeight() ){
			m_drawTexture.m_pTex2D->UpdateDrawInfo().m_posOrigin = candidatePos;
			DEBUG_PRINT( "敵生成 x = %f, y = %f\n", candidatePos.x, candidatePos.y );
			break;
		}
	}

	return InitMain();
}

bool EnemyBase::DieMain()
{
	if( !DieMainCustom() ){
		return false;
	}

	if( GameRegister::GetInstance()->GetManagerEnemy() ){
		GameRegister::GetInstance()->UpdateManagerEnemy()->RemoveEnemy( this );
	}
	
	m_textureLife.DeleteAndInit();
	SAFE_DELETE( m_pEnemyAI );

	// 倒された敵をカウントしておく
	GameRegister::GetInstance()->UpdateManagerGame()->AddDestroyCount();

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

		// AIによって更新された値を反映
		//m_drawTexture.m_pTex2D->SetDrawInfo( m_drawTexture.m_texInfo );

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
}

void EnemyBase::DrawUpdate()
{
	m_drawTexture.m_pTex2D->DrawUpdate2D();
	m_textureLife.m_pTex2D->DrawUpdate2D();
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

	case Common::EVENT_HIT_BULLET_PLAYER:	// Playerの弾に当たった
		HitPlayreBullet( eventId.m_eventValue );
		break;

	case Common::EVENT_HIT_BLADE_PLAYER:	// Playerの斬撃に当たった
		HitPlayreSlashing( eventId.m_eventValue );
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
	if( !m_drawTexture.m_pTex2D ){
		DEBUG_ASSERT( 0, "必要なクラスが作成されていない");
	}
	return m_drawTexture.m_pTex2D->GetDrawInfo();
}

const uint32_t &EnemyBase::GetEnemyLevel() const
{
	return m_enemyLv;
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
	m_stunTime = 10;
	GameEffect::CreateEffect( GameEffect::EFFECT_SLASHING_HIT, m_drawTexture.m_pTex2D->GetDrawInfo().m_posOrigin );
	UpdateEnemyDamage( damageValue );
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

	//switch( m_actionInfoAI.m_AItype ){
	//default:

	//	break;
	//case AI_SHOOT_BULLET:
	//	// m_actionInfoAI.m_AIInfoから情報を取り出していろいろ行う
	//	break;
	//}
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

	//ダメージエフェクト作成
	GameEffectDamage::GetInstance()->CreateEffectDamage( damageValue
		, static_cast<uint32_t>(m_drawTexture.m_pTex2D->GetDrawInfo().m_posOrigin.x)
		, static_cast<uint32_t>(m_drawTexture.m_pTex2D->GetDrawInfo().m_posOrigin.y));

	if( m_HP <= 0 ){
		// スコア追加
		Common::ENEMY_KIND kind = GetKind();
		switch( kind ){
		case Common::ENEMY_KIND_AAA:
			GameRecorder::GetInstance()->ScoreEvent( GameRecorder::ENEMY_AAA_DEATH );
			break;
		case Common::ENEMY_KIND_BBB:
			GameRecorder::GetInstance()->ScoreEvent( GameRecorder::ENEMY_BBB_DEATH );
			break;
		case Common::ENEMY_KIND_CCC:
			GameRecorder::GetInstance()->ScoreEvent( GameRecorder::ENEMY_CCC_DEATH );
			break;
		case Common::ENEMY_KIND_BOSS:
			GameRecorder::GetInstance()->ScoreEvent( GameRecorder::ENEMY_BOSS_DEATH );
			break;
		case Common::ENEMY_KIND_SLIME_KING:
			GameRecorder::GetInstance()->ScoreEvent( GameRecorder::ENEMY_SLIME_KING_DEATH );
			break;
		}

		if( Utility::GetRandamValue( 10, 0 ) == 0 ){
			Common::ITEM_KIND itemKind = static_cast<Common::ITEM_KIND>( Utility::GetRandamValue( Common::ITEM_KIND_MAX-1, 0 ) );
			GameRegister::GetInstance()->UpdateManagerGame()->CreateItem( itemKind, m_drawTexture.m_pTex2D->GetDrawInfo().m_posOrigin );
		}

		// 爆破エフェクトを出す
		GameEffect::CreateEffect( GameEffect::EFFECT_BOMB, m_drawTexture.m_pTex2D->GetDrawInfo().m_posOrigin );

		// 爆発SE鳴らす
		SoundManager::GetInstance()->PlaySE("Death");

		// 死亡
		TaskStartDie();
	}

	// 連続Hit数加算
	GameRecorder::GetInstance()->IncHitCounter();
}
