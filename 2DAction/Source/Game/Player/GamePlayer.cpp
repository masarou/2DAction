/* ====================================================================== */
/**
 * @brief  プレイヤークラス
 *
 * @note
 *		斬撃	Lv1~5	小斬撃
				Lv6~9	大斬撃

		ダッシュ	Lv2から使用可能
					Lv4でエフェクトがつく
					Lv9で押しっぱなしダッシュが可能に

		マシンガン	貫通にする？
					ため攻撃あり？

 */
/* ====================================================================== */

#include "GamePlayer.h"
#include "Game/Effect/GameEffect.h"

#include "System/Draw2D/SystemDraw2DResource.h"
#include "System/Sound/SystemSoundManager.h"
#include "System/Message/SystemMessageManager.h"
#include "Game/Enemy/EnemyManager.h"
#include "Game/Item/ItemObject.h"
#include "Game/GameRegister.h"
#include "Game/GameRecorder.h"
#include "Game/Attack/GameAttackGun.h"
#include "Game/Attack/GameAttackBlade.h"
#include "Common/Utility/CommonGameUtility.h"

#include "Game/Attack/Slashing.h"

#include "System/SystemFPSManager.h"

// 固定値
static uint32_t DAMAGE_INVISIBLE_TIME	= 40;

static uint32_t LIFE_POINT_DEFAULT_MAX	= 200;
static uint32_t MOVE_SPEED_DEFAULT		= 3;
static float DASH_MULTIPLY_DEFAULT		= 5.0f;
static uint32_t DASH_SPEED_MAX			= 15;
static uint32_t WARNING_LIFE			= 40;
static uint32_t EMERGENCY_LIFE			= 20;

static uint32_t BULLET_INTERBAL_MIN		= 1;
static uint32_t BULLET_DAMAGE_MAX		= 100;

// アニメタグ
static char *ANIM_TAG_UP	= "up";
static char *ANIM_TAG_DOWN	= "down";
static char *ANIM_TAG_RIGHT	= "right";
static char *ANIM_TAG_LEFT	= "left";

GamePlayer *GamePlayer::CreatePlayer()
{
	return NEW GamePlayer();
}

GamePlayer::GamePlayer(void)
: TaskUnit("Player")
, Collision2DUnit( "Player.json" )
, m_lifeLv( 0 )
, m_speedLv( 0 )
, m_defenceLv( 0 )
, m_playerLife( LIFE_POINT_DEFAULT_MAX )
, m_playerLifeMax( LIFE_POINT_DEFAULT_MAX )
, m_playerState( ABNORMAL_STATE_NONE )
, m_speedMove( 0 )
, m_speedMoveBase( MOVE_SPEED_DEFAULT )
, m_deffenceLate( 1.0f )
, m_speedMultiply( 0.0f )
, m_invisibleTime( 0 )
, m_invalidCtrlTime( 0 )
, m_attackGun(NULL)
, m_attackBlade(NULL)
, m_pStatusMenu(NULL)
{
	m_forceMoveInfo.Init();
	m_pStatusMenu = NEW PlayerStatusMenu();
}


GamePlayer::~GamePlayer(void)
{
}

bool GamePlayer::Init()
{
	// 描画クラスセットアップ
	TEX_DRAW_INFO drawInfo;
	drawInfo.m_prioity = Common::PRIORITY_ABOVE_NORMAL;
	drawInfo.m_usePlayerOffset = false;
	drawInfo.m_posOrigin = math::Vector2( WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f );
	drawInfo.m_fileName = "Player.json";
	m_drawTexture.m_pTex2D->SetDrawInfo( drawInfo );

	// 遠距離攻撃マシンガンクラスセット
	m_attackGun = AttackGun::CreateGun( Common::OWNER_PLAYER );
	
	// 近接攻撃剣クラスセット
	m_attackBlade = AttackBlade::CreateAttackBlade( Common::OWNER_PLAYER );

	// ベースとなるステータスとアイテム取得数を反映させる
	SetupInitPlayerState();

	// 画像の真ん中がオフセット位置になるように調整しておく(プレイヤーの初期位置セット)
	const TEX_INIT_INFO &playerTexInfo = TextureResourceManager::GetInstance()->GetLoadTextureInfo( m_drawTexture.m_pTex2D->GetDrawInfo().m_fileName.c_str() );
	GameAccesser::GetInstance()->SetPlayerOffSet( playerTexInfo.m_sizeWidth / 2.0f, playerTexInfo.m_sizeHeight / 2.0f );
	math::Vector2 startPos = GameRegister::GetInstance()->GetGameMap()->GetPlayerStartPos() - drawInfo.m_posOrigin;
	GameAccesser::GetInstance()->AddPlayerOffSet( startPos );

	if( m_speedLv == Common::STATUS_LEVEL_MAX ){
		SetPadButtonState( InputWatcher::BUTTON_R1, InputWatcher::EVENT_PRESS );
		SetPadButtonState( InputWatcher::BUTTON_L1, InputWatcher::EVENT_PRESS );
	}
	else{
		SetPadButtonState( InputWatcher::BUTTON_R1, InputWatcher::EVENT_PUSH );
		SetPadButtonState( InputWatcher::BUTTON_L1, InputWatcher::EVENT_PUSH );
	}
	return true;
}

bool GamePlayer::DieMain(){
	return true;
}

/* ================================================ */
/**
 * @brief	更新関連関数
 */
/* ================================================ */
void GamePlayer::Update()
{
	if( m_playerLife == 0 ){
		return;
	}

	{
		// 毎フレーム値が変わるような項目はここで

		// ダッシュ関連
		m_speedMove = m_speedMoveBase * static_cast<uint32_t>(m_speedMultiply + 0.5f);
		if( m_speedMultiply > 1.0f ){
			m_speedMultiply *= 0.94f;
			
			// 3フレームに一回残像エフェクト描画
			if( FpsManager::GetUpdateCounter() % 3 == 0 && m_speedLv >= 3 ){
				GameEffect::CreateEffect( GameEffect::EFFECT_DASH_SMOKE, Utility::GetPlayerPos() );
			}
		}
		// 一定数になったら切り捨て(終了)
		if( m_speedMultiply < 1.2f || m_speedLv == Common::STATUS_LEVEL_MAX ){
			m_speedMultiply = 1.0f;
		}
		// 早すぎる場合は切り捨て
		if( m_speedMove > DASH_SPEED_MAX ){
			m_speedMove = DASH_SPEED_MAX;
		}

		// 無敵時間中ならデクリメント
		if( m_invisibleTime > 0 ){
			--m_invisibleTime;
		}

		// 操作不能時間中ならデクリメント
		if( m_invalidCtrlTime > 0 ){
			--m_invalidCtrlTime;
		}
	}

	// 現在のライフセット
	if( m_pStatusMenu ){
		m_pStatusMenu->SetPlayerHP( m_playerLife, m_playerLifeMax );
	}

	// 操作不能中でないならパッドイベントをコール
	if( m_invalidCtrlTime == 0 ){
		// パッドに対応した挙動を設定
		CallPadEvent();
		
		// 攻撃判定
		if( GetStickInfoRight().m_vec != DEFAULT_VECTOR2 ){
			const TEX_DRAW_INFO &drawInfo = m_drawTexture.m_pTex2D->GetDrawInfo();
			math::Vector2 pos = math::Vector2( drawInfo.m_posOrigin.x, drawInfo.m_posOrigin.y ) + GameAccesser::GetInstance()->GetPlayerOffSet();
			math::Vector2 vec = GetStickInfoRight().m_vec;
			vec.Normalize();
			m_attackGun->ShootBullet( pos, vec );
		}
	}

	// 外からのダメージ等での強制移動
	if( m_forceMoveInfo.m_posX != 0.0f 
		&& m_forceMoveInfo.m_posY != 0.0f ){
		if( m_forceMoveInfo.m_forcePower <= 1.0f ){
			m_forceMoveInfo.Init();
		}
		else{
			math::Vector2 forceVec = math::Vector2( m_forceMoveInfo.m_posX, m_forceMoveInfo.m_posY );
			UpdateMove( forceVec*m_forceMoveInfo.m_forcePower, /*bool isForce=*/true );
			m_forceMoveInfo.m_forcePower *= 0.9f;
		}
	}

	// 再生アニメタグセット
	m_drawTexture.m_pTex2D->SetAnim(GetAnimTag());
}
void GamePlayer::DrawUpdate()
{
	if( m_invisibleTime % 3 == 1 ){
		// ダメージを受けない時間帯ならば3フレに一回描画せず点滅させる
	}
	else{
		// プレイヤー描画	
		m_drawTexture.m_pTex2D->DrawUpdate2D();
	}
}

/* ================================================ */
/**
 * @brief	パッド操作関数
 */
/* ================================================ */
void GamePlayer::PadEventUp()
{
	math::Vector2 moveVal = math::Vector2(0.0f, -1.0f*m_speedMove);
	UpdateMove( moveVal );
}

void GamePlayer::PadEventDown()
{
	math::Vector2 moveVal = math::Vector2(0.0f, 1.0f*m_speedMove);
	UpdateMove( moveVal );
}

void GamePlayer::PadEventRight()
{
	math::Vector2 moveVal = math::Vector2(1.0f*m_speedMove, 0.0f);
	UpdateMove( moveVal );
}

void GamePlayer::PadEventLeft()
{
	math::Vector2 moveVal = math::Vector2(-1.0f*m_speedMove, 0.0f);
	UpdateMove( moveVal );
}

void GamePlayer::UpdateMove( math::Vector2 &moveVec, bool isForce )
{
	// 斬撃攻撃中なら速度を落とす
	if( m_attackBlade && !isForce && m_attackBlade->IsSlashingAnimPlay() ){
		moveVec /= 2.0f;
	}

	// 移動先が歩けないならば移動しない
	if( !CanMoveThisPos( moveVec ) ){
		return;
	}

	GameAccesser::GetInstance()->AddPlayerOffSet( moveVec );
}

void GamePlayer::PadEventDecide()
{
	if( !m_attackBlade ){
		return;
	}

	// 斬撃
	const STICK_INFO &stickInfo = GetStickInfoLeft();
	if( stickInfo.m_vec == math::Vector2() ){
		math::Vector2 vec = math::Vector2();
		std::string anim = GetAnimTag();
		if( anim.compare("up") == 0 ){
			vec = math::Vector2( 0.0f, -1.0f );
		}
		if( anim.compare("down") == 0 ){
			vec = math::Vector2( 0.0f, 1.0f );
		}
		if( anim.compare("left") == 0 ){
			vec = math::Vector2( -1.0f, 0.0f );
		}
		if( anim.compare("right") == 0 ){
			vec = math::Vector2( 1.0f, 0.0f );
		}
		m_attackBlade->CreateSlashing( Utility::GetPlayerPos(), vec );
	}
	else{
		m_attackBlade->CreateSlashing( Utility::GetPlayerPos(), stickInfo.m_vec );
	}
}

void GamePlayer::PadEventR1()
{
	ActionPlayerDash();
}

void GamePlayer::PadEventL1()
{
	ActionPlayerDash();
}

/* ================================================ */
/**
 * @brief	情報取得関数
 */
/* ================================================ */
const TEX_DRAW_INFO &GamePlayer::GetDrawInfo() const
{
	if( m_drawTexture.m_pTex2D == NULL ){
		DEBUG_ASSERT( 0, "プレイヤーの描画クラスがNULL");
	}
	return m_drawTexture.m_pTex2D->GetDrawInfo();
}

const uint32_t	&GamePlayer::GetPlayerLifeMax() const
{
	return m_playerLifeMax;
}

/* ================================================ */
/**
 * @brief	タスクにイベント追加
 */
/* ================================================ */
void GamePlayer::AddEvent( const Common::CMN_EVENT &cmnEvent )
{
	SystemMessageManager::GetInstance()->PushMessage( GetUniqueId(), cmnEvent );
}

/* ================================================ */
/**
 * @brief	ダッシュ周りの処理まとめ関数
 */
/* ================================================ */
void GamePlayer::ActionPlayerDash()
{
	if( GetStickInfoLeft().m_vec == DEFAULT_VECTOR2 ){
		// 方向キーの入力が入っていないので何もしない
		return;
	}

	if( m_speedLv == 0 ){
		// ダッシュLvが0なので何もしない
		return;
	}

	// ダッシュLvが最大の場合は常に有効
	if( m_speedLv == Common::STATUS_LEVEL_MAX ){
		// 効果音は30fに一回鳴らす
		if( FpsManager::GetUpdateCounter() % 30 == 0 || !IsPreFrameButtonPress( InputWatcher::BUTTON_L1 ) ){
			SoundManager::GetInstance()->PlaySE("Dash");
		}
		// 係数セット
		m_speedMultiply = DASH_MULTIPLY_DEFAULT + static_cast<uint32_t>( 0.5 * m_speedLv );
	}
	else if( m_speedMultiply <= 1.0f ){
		SoundManager::GetInstance()->PlaySE("Dash");
		// 係数セット
		m_speedMultiply = DASH_MULTIPLY_DEFAULT + static_cast<uint32_t>( 0.5 * m_speedLv );
	}
}

/* ================================================ */
/**
 * @brief	現在のプレイヤーの状況から再生するアニメタグ取得
 */
/* ================================================ */
std::string GamePlayer::GetAnimTag()
{
	// どの方向をむいているか
	const STICK_INFO &stickInfo = GetStickInfoLeft();
	std::string retAnim = "";

	switch( Utility::GetDirection( stickInfo.m_vec.x, stickInfo.m_vec.y ) ){
	default:
		break;
	case InputWatcher::BUTTON_UP:
		retAnim = "up";
		break;
	case InputWatcher::BUTTON_DOWN:
		retAnim = "down";
		break;
	case InputWatcher::BUTTON_LEFT:
		retAnim = "left";
		break;
	case InputWatcher::BUTTON_RIGHT:
		retAnim = "right";
		break;
	}

	if( retAnim.compare("") == 0 ){
		retAnim = m_drawTexture.m_pTex2D->GetPlayAnim();
	}
	return retAnim;
}

/* ================================================ */
/**
 * @brief	ベースとなるステータス＋取得アイテム数を反映させる
 */
/* ================================================ */
void GamePlayer::SetupInitPlayerState()
{
	// ベースステータスを反映
	Common::SAVE_DATA playData;
	Utility::GetSaveData( playData );

	// ライフを反映
	m_lifeLv		= playData.m_playerBaseStateLv[Common::BASE_STATE_LIFE];
	m_playerLifeMax	= LIFE_POINT_DEFAULT_MAX + Utility::ConvertLevelToBaseState( Common::BASE_STATE_LIFE, m_lifeLv );
	m_playerLife	= m_playerLifeMax;

	// 行動速度を反映
	m_speedLv		= playData.m_playerBaseStateLv[Common::BASE_STATE_MOVE_SPEED];
	m_speedMoveBase = MOVE_SPEED_DEFAULT + Utility::ConvertLevelToBaseState( Common::BASE_STATE_MOVE_SPEED, m_speedLv );

	// 防御力を反映
	m_defenceLv		= playData.m_playerBaseStateLv[Common::BASE_STATE_DEFFENCE];
	m_deffenceLate = ( 100.0f - static_cast<float>( Utility::ConvertLevelToBaseState( Common::BASE_STATE_DEFFENCE, m_defenceLv ) ) ) / 100.0f;

	// マシンガンクラスを反映
	if( m_attackGun ){
		m_attackGun->SetGunLevel( playData.m_playerBaseStateLv[Common::BASE_STATE_BULLET_DMG], playData.m_playerBaseStateLv[Common::BASE_STATE_BULLET_SPD] );
	}

	// 剣クラスを反映
	if( m_attackBlade ){
		m_attackBlade->SetBladeLevel( playData.m_playerBaseStateLv[Common::BASE_STATE_BLADE_LEVEL] );
	}

	// アイテム取得数を反映
	for( uint32_t i = 0; i < Common::ITEM_KIND_MAX ; ++i ){
		uint32_t itemNum = GameRecorder::GetInstance()->GetItemCount( static_cast<Common::ITEM_KIND>(i) );
		for( uint32_t j = 0; j < itemNum ; ++j ){
			PlayerGetItem( static_cast<Common::ITEM_KIND>(i), /*isCountUp=*/false );
		}
	}
}

/* ================================================ */
/**
 * @brief	現在のステータスでプレイヤーが引数のベクター分移動ができるかどうかチェック
 */
/* ================================================ */
bool GamePlayer::CanMoveThisPos( const math::Vector2 &nextFlameAddValue ) const
{
	bool ret = false;

	const TEX_INIT_INFO &playerTexInfo = TextureResourceManager::GetInstance()->GetLoadTextureInfo( m_drawTexture.m_pTex2D->GetDrawInfo().m_fileName.c_str() );

	math::Vector2 nextFlamePos;
	GameAccesser::GetInstance()->GetPlayerOffSet( nextFlamePos.x, nextFlamePos.y );

	// offsetの数値分位置を引いて描画しているので実際に調べるときは足してやる
	// さらにプレイヤー画像の画像中心位置になるように足してやる
	nextFlamePos += nextFlameAddValue;
	nextFlamePos.x += static_cast<float>(WINDOW_WIDTH / 2.0f);
	nextFlamePos.y += static_cast<float>(WINDOW_HEIGHT / 2.0f);

	// 移動先が歩けないならば移動しない
	if( Utility::IsMovable( m_drawTexture.m_pTex2D->GetDrawInfo().m_fileName, nextFlamePos ) ){
		ret = true;
	}
	return ret;
}


/* ================================================ */
/**
 * @brief	他のクラスからのイベントコール
 */
/* ================================================ */
void GamePlayer::EventUpdate( Common::CMN_EVENT &eventId )
{
	switch( eventId.m_event ){
	case Common::EVENT_HIT_ENEMY_SLIME:
	case Common::EVENT_HIT_ENEMY_AHRIMAN:
	case Common::EVENT_HIT_ENEMY_COW:
	case Common::EVENT_HIT_ENEMY_BOSS:
	case Common::EVENT_HIT_ENEMY_SLIME_KING:
	case Common::EVENT_HIT_ENEMY_WIZARD:
	case Common::EVENT_HIT_WIZARD_CRYSTAL:
	case Common::EVENT_HIT_BULLET_ENEMY:
	case Common::EVENT_HIT_BLADE_ENEMY:
	case Common::EVENT_HIT_EXPLOSION_ENEMY:
		if( m_invisibleTime == 0 ){
			EventDamage( eventId );
		}
		break;
	case Common::EVENT_GET_ITEM_BULLET:
		PlayerGetItem( Common::ITEM_KIND_RAPID_BULLET );
		break;
	case Common::EVENT_GET_ITEM_LIFE:
		PlayerGetItem( Common::ITEM_KIND_LIFE_UP );
		break;
	case Common::EVENT_GET_ITEM_DAMAGE:
		PlayerGetItem( Common::ITEM_KIND_DAMAGE_UP );
		break;
	case Common::EVENT_GET_ITEM_BATTLE_POINT:
		PlayerGetItem( Common::ITEM_KIND_BATTLE_POINT );
		break;
	case Common::EVENT_ADD_FORCE_MOVE:
		// 必要な情報を取り出す
		m_forceMoveInfo = eventId.GetExInfoForceMove();
		m_invalidCtrlTime	= 10;
		m_invisibleTime		= 60;
		
		SetPlayerState( ABNORMAL_STATE_MOVE_LOCK, false );
		break;
	default:

		break;
	}
}

/* ================================================ */
/**
 * @brief	イベントに対応した関数群
 */
/* ================================================ */

void GamePlayer::EventDamage( Common::CMN_EVENT &eventId )
{
	Common::EVENT_MESSAGE	eventKind	= eventId.m_event;
	uint32_t				damageValue	= eventId.m_eventValue;

	// ダメージ音
	switch( eventKind ){
	case Common::EVENT_HIT_BULLET_ENEMY:
		SoundManager::GetInstance()->PlaySE("DamageBullet");
		break;
	default:
		SoundManager::GetInstance()->PlaySE("DamageDirect");
		break;
	}

	// ダメージを受けたら一定時間ダメージを受けない
	m_invisibleTime = DAMAGE_INVISIBLE_TIME;

	// 各ダメージ種類による特殊処理
	switch( eventKind ){
	case Common::EVENT_HIT_ENEMY_SLIME_KING:
		{
			if( !IsPlayerState( ABNORMAL_STATE_MOVE_LOCK ) ){
				// 三秒ほど動けずとりこまれる、その後はじき出される
				Common::CMN_EVENT forceEvent;
				forceEvent.Init();
				forceEvent.m_event = Common::EVENT_ADD_FORCE_MOVE;
				forceEvent.m_delayTime = 120;
				Common::EX_FORCE_MOVE moveInfo;
				math::Vector2 vec	= math::Vector2( Utility::GetRandamValueFloat( 100, -100 ) / 100.0f, Utility::GetRandamValueFloat( 100, -100 ) / 100.0f );
				vec.Normalize();
				moveInfo.m_posX = vec.x;
				moveInfo.m_posY = vec.y;
				moveInfo.m_forcePower	= 20.0f;
				forceEvent.SetExInfoForceMove( moveInfo );
				AddEvent( forceEvent );

				// 動けないステータスにセット
				SetPlayerState( ABNORMAL_STATE_MOVE_LOCK, true );
			}
			else{
				// 既にロック状態なのでなにもしない(ダメージ処理だけ)
			}
		}
		break;
	case Common::EVENT_HIT_ENEMY_SLIME:
	case Common::EVENT_HIT_ENEMY_AHRIMAN:
	case Common::EVENT_HIT_ENEMY_COW:
	case Common::EVENT_HIT_ENEMY_BOSS:
	case Common::EVENT_HIT_ENEMY_WIZARD:
	case Common::EVENT_HIT_WIZARD_CRYSTAL:
		{
			// 吹き飛ぶ方向を設定してイベントとしてセットしておく
			math::Vector2 plPos		= Utility::GetPlayerPos();
			math::Vector2 enemyPos	= math::Vector2( eventId.GetExInfoCmn().m_posX, eventId.GetExInfoCmn().m_posY );			
			math::Vector2 forceAngle = plPos - enemyPos;
			forceAngle.Normalize();

			Common::CMN_EVENT forceEvent;
			forceEvent.Init();
			forceEvent.m_event = Common::EVENT_ADD_FORCE_MOVE;
			Common::EX_FORCE_MOVE moveInfo;
			moveInfo.m_posX = forceAngle.x;
			moveInfo.m_posY = forceAngle.y;
			moveInfo.m_forcePower	= ( eventKind == Common::EVENT_HIT_ENEMY_COW ) ? 25.0f : 10.0f ;
			forceEvent.SetExInfoForceMove( moveInfo );
			AddEvent( forceEvent );
		}
		break;
	default:
		break;
	}

	// 特定の攻撃を受けたら一定時間操作不能
	switch( eventKind ){
	case Common::EVENT_HIT_BLADE_ENEMY:
		m_invalidCtrlTime = 10;
		break;
	case Common::EVENT_HIT_ENEMY_SLIME_KING:
		m_invalidCtrlTime = 200;
		break;
	default:
		break;
	}

	// 防御力に応じてダメージを減らす
	uint32_t totalDamage = static_cast<uint32_t>( damageValue * m_deffenceLate );

	// ライフを減らす
	if( m_playerLife > totalDamage ){
		m_playerLife -= totalDamage;
	}
	else{
		m_playerLife = 0;
	}

	// ライフ残量によってSEを鳴らす
	if( m_playerLife == 0 ){

	}
	else if( m_playerLife <= EMERGENCY_LIFE ){
		// HPが少ない警告を鳴らす
		SoundManager::GetInstance()->PlaySE("Emergency");
	}
	else if( m_playerLife <= WARNING_LIFE ){
		// HPが少ない警告を鳴らす
		SoundManager::GetInstance()->PlaySE("Warning");
	}
}

// アイテム取得
void GamePlayer::PlayerGetItem( const Common::ITEM_KIND &itemKind, bool isCountUp )
{
	// アイテムレベル描画クラスに取得を知らせるかどうか
	bool reflectDisp = true;

	switch( itemKind ){
	default:
	case Common::ITEM_KIND_RAPID_BULLET:
		{
			// 銃の発射間隔を狭める
			AttackGun::GunState &gunState = m_attackGun->UpdateGunState();
			if( gunState.m_shootInterval > BULLET_INTERBAL_MIN ){
				gunState.m_shootInterval -= 1;
			}
			else{
				reflectDisp = false;
			}

			if( isCountUp ){
				// 取得アイテム数をカウント
				GameRecorder::GetInstance()->AddItem( Common::ITEM_KIND_RAPID_BULLET );
				// アイテム取得音を鳴らす
				SoundManager::GetInstance()->PlaySE("GetItem");
			}
		}
		break;
	case Common::ITEM_KIND_LIFE_UP:
		{
			// ライフ回復
			m_playerLife += 30;
			if( m_playerLife > m_playerLifeMax ){
				m_playerLife = m_playerLifeMax;
			}

			if( isCountUp ){
				// 取得アイテム数をカウント
				GameRecorder::GetInstance()->AddItem( Common::ITEM_KIND_LIFE_UP );
				// アイテム取得音を鳴らす
				SoundManager::GetInstance()->PlaySE("GetItemHeal");
			}
		}
		break;
	case Common::ITEM_KIND_DAMAGE_UP:
		{
			// ダメージ量UP
			AttackGun::GunState &gunState = m_attackGun->UpdateGunState();
			if( gunState.m_damage < BULLET_DAMAGE_MAX ){
				gunState.m_damage += 5;
			}
			else{
				reflectDisp = false;
			}

			if( gunState.m_damage % 4 == 0 ){// 20の倍数になるごとに斬撃の攻撃力もUP
				AttackBlade::BladeState &bladeState = m_attackBlade->UpdateBladeState();
				bladeState.m_damage += 5;
			}

			if( isCountUp ){
				// 取得アイテム数をカウント
				GameRecorder::GetInstance()->AddItem( Common::ITEM_KIND_DAMAGE_UP );
				// アイテム取得音を鳴らす
				SoundManager::GetInstance()->PlaySE("GetItem");
			}
		}
		break;
	case Common::ITEM_KIND_BATTLE_POINT:
		if( isCountUp ){
			// 取得アイテム数をカウント
			GameRecorder::GetInstance()->AddItem( Common::ITEM_KIND_BATTLE_POINT );
		}
		break;
	}

	// 表示部分に伝える
	if( m_pStatusMenu && reflectDisp ){
		m_pStatusMenu->AddItemLevel( itemKind );
	}
}


bool GamePlayer::IsPlayerState( const PLAYER_ABNORMAL_STATE &checkState ) const
{
	if( m_playerState & checkState ){
		return true;
	}
	return false;
}

void GamePlayer::SetPlayerState( const PLAYER_ABNORMAL_STATE &checkState, const bool &flag )
{
	if( flag ){
		m_playerState |= checkState;
	}
	else{
		m_playerState &= ~checkState;
	}
}
