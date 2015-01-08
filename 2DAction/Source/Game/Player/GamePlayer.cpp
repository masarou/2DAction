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
#include "Game/GameRegister.h"
#include "Common/Utility/CommonGameUtility.h"

static uint32_t DAMAGE_INVISIBLE_TIME = 120;
static uint32_t LIFE_POINT_MAX = 100;

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
, m_speedMove( 0 )
, m_speedMoveBase( 3 )
, m_speedMultiply( 0.0f )
, m_invisibleTime(0)
, m_playerLife(LIFE_POINT_MAX)
{
}


GamePlayer::~GamePlayer(void)
{
}

bool GamePlayer::Init()
{
	// 描画クラスセットアップ
	m_texturePlayer.Init();
	m_texturePlayer.m_pTex2D = NEW Game2DBase("player.json");
	m_texturePlayer.m_texInfo.m_prioity = PRIORITY_ABOVE_NORMAL;
	m_texturePlayer.m_texInfo.m_usePlayerOffset = false;
	m_texturePlayer.m_texInfo.m_posOrigin = math::Vector2( WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f );
	m_texturePlayer.m_texInfo.m_fileName = "player.json";
	GetBelongAreaInMap( m_texturePlayer.m_texInfo );
	m_texturePlayer.m_pTex2D->SetDrawInfo(m_texturePlayer.m_texInfo);

	// 描画クラスセットアップ
	m_textureLife.Init();
	m_textureLife.m_pTex2D = NEW Game2DBase("lifeGauge.json");
	m_textureLife.m_texInfo.Init();
	m_textureLife.m_texInfo.m_prioity = PRIORITY_ABOVE_NORMAL;
	m_textureLife.m_texInfo.m_usePlayerOffset = false;
	m_textureLife.m_texInfo.m_posOrigin.x = 0.0f;
	const TEX_INIT_INFO &lifeTexInfo = TextureResourceManager::GetInstance()->GetLoadTextureInfo( "lifeGauge.json" );
	m_textureLife.m_texInfo.m_posOrigin.y = static_cast<float>(WINDOW_HEIGHT - lifeTexInfo.m_sizeHeight);
	m_textureLife.m_texInfo.m_arrangeOrigin = math::Vector2( 0.0f, 0.0f );
	m_textureLife.m_pTex2D->SetDrawInfo(m_textureLife.m_texInfo);

	m_textureLifeFrame.Init();
	m_textureLifeFrame.m_pTex2D = NEW Game2DBase("lifeFrame.json");
	m_textureLifeFrame.m_texInfo.m_prioity = PRIORITY_HIGH;
	m_textureLifeFrame.m_texInfo.m_usePlayerOffset = false;
	m_textureLifeFrame.m_texInfo.m_posOrigin = m_textureLife.m_texInfo.m_posOrigin;
	m_textureLifeFrame.m_texInfo.m_arrangeOrigin = m_textureLife.m_texInfo.m_arrangeOrigin;
	m_textureLifeFrame.m_pTex2D->SetDrawInfo(m_textureLifeFrame.m_texInfo);

	// 攻撃マシンガンクラスセット
	m_attackGun = AttackGun::CreateGun( Common::OWNER_PLAYER );

	// 画像の真ん中がオフセット位置になるように調整しておく
	const TEX_INIT_INFO &playerTexInfo = TextureResourceManager::GetInstance()->GetLoadTextureInfo( m_texturePlayer.m_texInfo.m_fileName.c_str() );
	GameAccesser::GetInstance()->SetPlayerOffSet( playerTexInfo.m_sizeWidth / 2.0f, playerTexInfo.m_sizeHeight / 2.0f );

	return true;
}

void GamePlayer::Update()
{
	if( m_playerLife == 0 ){
		return;
	}

	{
		// マイフレーム値が変わるような項目はここで
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
	}

	// パッドに対応した挙動を設定
	CallPadEvent();

	// 再生アニメタグセット
	m_texturePlayer.m_pTex2D->SetAnim(GetAnimTag());

	// 攻撃判定
	if( GetStickInfoRight().m_vec != DEFAULT_VECTOR2 ){
		math::Vector2 pos = math::Vector2( m_texturePlayer.m_texInfo.m_posOrigin.x, m_texturePlayer.m_texInfo.m_posOrigin.y ) + GameAccesser::GetInstance()->GetPlayerOffSet();
		math::Vector2 vec = GetStickInfoRight().m_vec;
		vec.Normalize();
		m_attackGun->ShootBullet( pos, vec );
	}
}

void GamePlayer::CollisionUpdate()
{
	// アイテム、敵との当たり判定は各マネージャクラスが行い、
	// イベントをpushしてくれるのでここでは何もしない
}

void GamePlayer::DrawUpdate()
{
	if( m_invisibleTime % 3 == 1 ){
		// ダメージを受けない時間帯ならば3フレに一回描画せず点滅させる
	}
	else{
		// プレイヤー描画	
		m_texturePlayer.m_pTex2D->SetDrawInfo(m_texturePlayer.m_texInfo);
		m_texturePlayer.m_pTex2D->DrawUpdate2D();
	}

	// ライフゲージ描画
	{
		const TEX_INIT_INFO &lifeTexInfo = TextureResourceManager::GetInstance()->GetLoadTextureInfo( m_textureLife.m_texInfo.m_fileName.c_str() );
		float ratio = static_cast<float>(m_playerLife)/static_cast<float>(LIFE_POINT_MAX);
		float lifeValue = (WINDOW_WIDTH / lifeTexInfo.m_sizeWidth) * ratio;
		
		if( math::Absf( m_textureLife.m_texInfo.m_scale.x - lifeValue ) > 0.2f ){
			m_textureLife.m_texInfo.m_scale.x *= (m_textureLife.m_texInfo.m_scale.x - lifeValue < 0.0f) ? 1.02f : 0.98f ;
		}
		else{
			m_textureLife.m_texInfo.m_scale.x = lifeValue;
		}

		m_textureLife.m_pTex2D->SetDrawInfo(m_textureLife.m_texInfo);
		m_textureLife.m_pTex2D->DrawUpdate2D();
		m_textureLifeFrame.m_pTex2D->DrawUpdate2D();
	}
}

bool GamePlayer::DieMain(){
	m_texturePlayer.DeleteAndInit();
	m_textureLife.DeleteAndInit();
	m_textureLifeFrame.DeleteAndInit();
	return true;
}

/* ================================================ */
/**
 * @brief	パッド操作関数
 */
/* ================================================ */
void GamePlayer::PadEventUp()
{
	math::Vector2 moveVal = math::Vector2(0.0f, -1.0f*m_speedMove);

	// 移動先が歩けないならば移動しない
	if( !CanMoveThisPos( moveVal ) ){
		return;
	}
	GameAccesser::GetInstance()->AddPlayerOffSet( moveVal );
}

void GamePlayer::PadEventDown()
{
	math::Vector2 moveVal = math::Vector2(0.0f, 1.0f*m_speedMove);

	// 移動先が歩けないならば移動しない
	if( !CanMoveThisPos( moveVal ) ){
		return;
	}
	GameAccesser::GetInstance()->AddPlayerOffSet( moveVal );
}

void GamePlayer::PadEventRight()
{
	math::Vector2 moveVal = math::Vector2(1.0f*m_speedMove, 0.0f);

	// 移動先が歩けないならば移動しない
	if( !CanMoveThisPos( moveVal ) ){
		return;
	}
	GameAccesser::GetInstance()->AddPlayerOffSet(1.0f*m_speedMove, 0.0f);
}

void GamePlayer::PadEventLeft()
{
	math::Vector2 moveVal = math::Vector2(-1.0f*m_speedMove, 0.0f);

	// 移動先が歩けないならば移動しない
	if( !CanMoveThisPos( moveVal ) ){
		return;
	}

	GameAccesser::GetInstance()->AddPlayerOffSet( moveVal );
}

void GamePlayer::PadEventDecide()
{
}

void GamePlayer::PadEventCancel()
{
}

void GamePlayer::PadEventR1()
{
	m_speedMultiply = 7.0f;
}

void GamePlayer::PadEventL1()
{
	m_speedMultiply = 7.0f;
}

/* ================================================ */
/**
 * @brief	情報取得関数
 */
/* ================================================ */
const TEX_DRAW_INFO &GamePlayer::GetDrawInfo() const
{
	if( m_texturePlayer.m_pTex2D == NULL ){
		DEBUG_ASSERT( 0, "プレイヤーの描画クラスがNULL");
	}
	return m_texturePlayer.m_pTex2D->GetDrawInfo();
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

	switch( GetDirection( stickInfo.m_vec.x, stickInfo.m_vec.y ) ){
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
		retAnim = m_texturePlayer.m_pTex2D->GetPlayAnim();
	}
	return retAnim;
}

/* ================================================ */
/**
 * @brief	現在のステータスでプレイヤーが引数のベクター分移動ができるかどうかチェック
 */
/* ================================================ */
bool GamePlayer::CanMoveThisPos( const math::Vector2 &nextFlameAddValue )
{
	bool ret = false;

	const TEX_INIT_INFO &playerTexInfo = TextureResourceManager::GetInstance()->GetLoadTextureInfo( m_texturePlayer.m_texInfo.m_fileName.c_str() );

	math::Vector2 nextFlamePos;
	GameAccesser::GetInstance()->GetPlayerOffSet( nextFlamePos.x, nextFlamePos.y );

	// offsetの数値分位置を引いて描画しているので実際に調べるときは足してやる
	// さらにプレイヤー画像の画像中心位置になるように足してやる
	nextFlamePos += nextFlameAddValue;
	nextFlamePos.x += static_cast<float>(WINDOW_WIDTH / 2.0f) + (playerTexInfo.m_sizeWidth / 2.0f);
	nextFlamePos.y += static_cast<float>(WINDOW_HEIGHT / 2.0f) + (playerTexInfo.m_sizeHeight / 2.0f);

	// 移動先が歩けないならば移動しない
	if( GetMapHeight( nextFlamePos ) == 0 ){
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
	case Common::EVENT_HIT_ENEMY:
		if( m_invisibleTime == 0 ){
			EventDamage();
		}
		break;
	case Common::EVENT_GET_ITEM:
		PlayerGetItem( static_cast<ItemObject::ITEM_KIND>(eventId.m_eventValue) );
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

// 敵と接触した
void GamePlayer::EventDamage()
{
	// ダメージ音
	SoundManager::GetInstance()->PlaySE("Damage");

	// ダメージを受けたら一定時間ダメージを受けない
	m_invisibleTime = DAMAGE_INVISIBLE_TIME;

	// ライフを減らす
	if( m_playerLife > 50 ){
		m_playerLife -= 50;
	}
	else{
		m_playerLife = 0;
	}
}

// アイテム取得
void GamePlayer::PlayerGetItem( const ItemObject::ITEM_KIND &itemKind )
{
	switch( itemKind ){
	default:
	case ItemObject::ITEM_KIND_RAPID_BULLET:
		{
			// 銃の発射間隔を狭める
			AttackGun::GunState &gunState = m_attackGun->UpdateGunState();
			gunState.m_shootInterval = ( gunState.m_shootInterval < 2 ) ? 0 : gunState.m_shootInterval - 2;
		}
		break;
	}
}
