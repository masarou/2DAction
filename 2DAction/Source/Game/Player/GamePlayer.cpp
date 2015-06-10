/* ====================================================================== */
/**
 * @brief  プレイヤークラス
 *
 * @note
 *		
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

// 固定値
static uint32_t DAMAGE_INVISIBLE_TIME	= 60;

static uint32_t LIFE_POINT_DEFAULT_MAX	= 200;
static uint32_t MOVE_SPEED_DEFAULT		= 3;
static float DASH_TIME_DEFAULT			= 5.0f;
static uint32_t WARNING_LIFE			= 40;
static uint32_t EMERGENCY_LIFE			= 20;

static uint32_t BULLET_INTERBAL_MIN		= 1;
static uint32_t BULLET_DAMAGE_MAX		= 100;

static uint32_t DEFAULT_POS_X			= 1000;
static uint32_t DEFAULT_POS_Y			= 1000;


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
, m_speedMove( 0 )
, m_speedMoveBase( MOVE_SPEED_DEFAULT )
, m_speedMultiply( 0.0f )
, m_invisibleTime(0)
, m_invalidCtrlTime(0)
, m_playerLife( LIFE_POINT_DEFAULT_MAX )
, m_playerLifeMax( LIFE_POINT_DEFAULT_MAX )
, m_pStatusMenu(NULL)
{
	m_pStatusMenu = NEW PlayerStatusMenu();
}


GamePlayer::~GamePlayer(void)
{
}

bool GamePlayer::Init()
{
	// 描画クラスセットアップ
	TEX_DRAW_INFO drawInfo;
	drawInfo.m_prioity = PRIORITY_ABOVE_NORMAL;
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
	math::Vector2 vec = math::Vector2( static_cast<float>(DEFAULT_POS_X), static_cast<float>(DEFAULT_POS_Y) );
	GameAccesser::GetInstance()->AddPlayerOffSet( vec );

	SetPadButtonState( InputWatcher::BUTTON_R1, InputWatcher::EVENT_PRESS );
	SetPadButtonState( InputWatcher::BUTTON_L1, InputWatcher::EVENT_PRESS );

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
		m_speedMove = m_speedMoveBase * static_cast<uint32_t>(m_speedMultiply + 0.5f);
		if( m_speedMultiply > 1.0f ){
			m_speedMultiply *= 0.95f;
		}
		if( m_speedMultiply < 1.0f ){
			m_speedMultiply = 1.0f;
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
	}

	// 再生アニメタグセット
	m_drawTexture.m_pTex2D->SetAnim(GetAnimTag());

	// 攻撃判定
	if( GetStickInfoRight().m_vec != DEFAULT_VECTOR2 ){
		const TEX_DRAW_INFO &drawInfo = m_drawTexture.m_pTex2D->GetDrawInfo();
		math::Vector2 pos = math::Vector2( drawInfo.m_posOrigin.x, drawInfo.m_posOrigin.y ) + GameAccesser::GetInstance()->GetPlayerOffSet();
		math::Vector2 vec = GetStickInfoRight().m_vec;
		vec.Normalize();
		m_attackGun->ShootBullet( pos, vec );
	}
}
void GamePlayer::DrawUpdate()
{
	if( m_invisibleTime % 3 == 1 ){
		// ダメージを受けない時間帯ならば3フレに一回描画せず点滅させる
	}
	else{
		// プレイヤー描画	
		//m_drawTexture.m_pTex2D->SetDrawInfo(m_drawTexture.m_texInfo);
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

void GamePlayer::UpdateMove( math::Vector2 &moveVec )
{
	// 斬撃攻撃中なら移動できない
	if( m_attackBlade && m_attackBlade->IsSlashingAnimPlay() ){
		return;
	}

	//if( GetStickInfoRight().m_vec != DEFAULT_VECTOR2 ){
	//	// 攻撃中なら移動速度半分
	//	moveVec /= 2.0f;
	//}

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

void GamePlayer::PadEventCancel()
{
}

void GamePlayer::PadEventR1()
{
	m_speedMultiply = 5.0f;

	// ダッシュ効果音
	//SoundManager::GetInstance()->PlaySE("Dash");
}

void GamePlayer::PadEventL1()
{
	m_speedMultiply = 5.0f;

	// ダッシュ効果音
	//SoundManager::GetInstance()->PlaySE("Dash");
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
	m_playerLifeMax = ConvertLevelToBaseState( Common::BASE_STATE_LIFE, playData.m_playerBaseStateLv[Common::BASE_STATE_LIFE] );
	m_playerLife = m_playerLifeMax;

	// 行動速度を反映
	m_speedMoveBase = ConvertLevelToBaseState( Common::BASE_STATE_MOVE_SPEED, playData.m_playerBaseStateLv[Common::BASE_STATE_MOVE_SPEED] );

	// マシンガンクラスを反映
	if( m_attackGun ){
		AttackGun::GunState &gunState	= m_attackGun->UpdateGunState();
		gunState.m_damage				= ConvertLevelToBaseState( Common::BASE_STATE_BULLET_DMG, playData.m_playerBaseStateLv[Common::BASE_STATE_BULLET_DMG] );
		gunState.m_shootInterval		= ConvertLevelToBaseState( Common::BASE_STATE_BULLET_SPD, playData.m_playerBaseStateLv[Common::BASE_STATE_BULLET_SPD] );
		gunState.m_speed				+= playData.m_playerBaseStateLv[Common::BASE_STATE_BULLET_DMG];
	}

	// 剣クラスを反映
	if( m_attackBlade ){
		AttackBlade::BladeState &bladeState	= m_attackBlade->UpdateBladeState();
		bladeState.m_damage					= ConvertLevelToBaseState( Common::BASE_STATE_BLADE_DMG, playData.m_playerBaseStateLv[Common::BASE_STATE_BLADE_DMG] );
		bladeState.m_interval				= ConvertLevelToBaseState( Common::BASE_STATE_BLADE_SPD, playData.m_playerBaseStateLv[Common::BASE_STATE_BLADE_SPD] );
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
 * @brief	ステータスレベルから実際にセットする値へ変換
 */
/* ================================================ */
uint32_t GamePlayer::ConvertLevelToBaseState( Common::PLAYER_BASE_STATE stateKind, uint32_t level )
{
	uint32_t retVal = 0;
	switch( stateKind ){

	case Common::BASE_STATE_LIFE:
		retVal = LIFE_POINT_DEFAULT_MAX + (level*20);
		break;
	case Common::BASE_STATE_MOVE_SPEED:
		retVal = MOVE_SPEED_DEFAULT + level;
		break;
	case Common::BASE_STATE_DEFFENCE:
		retVal = 0;	// まだ未実装
		break;
	case Common::BASE_STATE_BLADE_SPD:
		retVal = SLASHING_INTERBAL_DEFAULT - (level*2);
		break;
	case Common::BASE_STATE_BLADE_DMG:
		retVal = SLASHING_DAMAGE_DEFAULT + (level*5);
		break;
	case Common::BASE_STATE_BULLET_SPD:
		retVal = SHOOT_INTERBAL_DEFAULT + level;
		break;
	case Common::BASE_STATE_BULLET_DMG:
		retVal = SHOOT_DAMAGE_DEFAULT + (level*5);
		break;
	}
	return retVal;
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

	math::Vector2 up = nextFlamePos;
	up.y -= playerTexInfo.m_sizeHeight/2.0f;
	math::Vector2 down = nextFlamePos;
	down.y += playerTexInfo.m_sizeHeight/2.0f;
	math::Vector2 left = nextFlamePos;
	left.x -= playerTexInfo.m_sizeWidth/2.0f;
	math::Vector2 right = nextFlamePos;
	right.x += playerTexInfo.m_sizeWidth/2.0f;

	// 移動先が歩けないならば移動しない
	if( Utility::GetMapHeight( up ) == 0
		&& Utility::GetMapHeight( down ) == 0
		&& Utility::GetMapHeight( left ) == 0
		&& Utility::GetMapHeight( right ) == 0){
		ret = true;
	}
	return ret;
}


/* ================================================ */
/**
 * @brief	他のクラスからのイベントコール
 */
/* ================================================ */
void GamePlayer::EventUpdate( const Common::CMN_EVENT &eventId )
{
	switch( eventId.m_event ){
	case Common::EVENT_HIT_ENEMY_AAA:
	case Common::EVENT_HIT_ENEMY_BBB:
	case Common::EVENT_HIT_ENEMY_CCC:
	case Common::EVENT_HIT_BULLET_ENEMY:
	case Common::EVENT_HIT_BLADE_ENEMY:
	case Common::EVENT_HIT_EXPLOSION_ENEMY:
		if( m_invisibleTime == 0 ){
			EventDamage( eventId.m_event, eventId.m_eventValue );
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
	default:

		break;
	}

}

/* ================================================ */
/**
 * @brief	イベントに対応した関数群
 */
/* ================================================ */

void GamePlayer::EventDamage( const Common::EVENT_MESSAGE &eventKind, const uint32_t &damageValue )
{
	// ダメージ音
	if( eventKind != Common::EVENT_HIT_BULLET_ENEMY ){
		SoundManager::GetInstance()->PlaySE("DamageDirect");
	}
	else{
		SoundManager::GetInstance()->PlaySE("DamageBullet");
	}

	// ダメージを受けたら一定時間ダメージを受けない
	m_invisibleTime = DAMAGE_INVISIBLE_TIME;

	// 斬撃攻撃を受けたら一定時間操作不能
	if( eventKind == Common::EVENT_HIT_BLADE_ENEMY ){
		m_invalidCtrlTime = 10;
	}

	// ライフを減らす
	if( m_playerLife > damageValue ){
		m_playerLife -= damageValue;
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
