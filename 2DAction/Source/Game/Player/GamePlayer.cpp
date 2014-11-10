

#include "GamePlayer.h"
#include "Game/Effect/GameEffect.h"

#include "System/Draw2D/SystemDraw2DResource.h"
#include "System/Sound/SystemSoundManager.h"
#include "Game/Enemy/EnemyManager.h"
#include "Game/GameRegister.h"

static uint32_t DAMAGE_INVISIBLE_TIME = 180;
static uint32_t LIFE_POINT_MAX = 100;

GamePlayer *GamePlayer::CreatePlayer()
{
	return NEW GamePlayer();
}

GamePlayer::GamePlayer(void)
: TaskUnit("Player")
, m_invisibleTime(0)
, m_playerLife(LIFE_POINT_MAX)
{
	// 描画クラスセットアップ
	m_player2D = NEW Game2DBase("player.json");
	m_playerInfo.Init();
	m_playerInfo.m_prioity = PRIORITY_ABOVE_NORMAL;
	m_playerInfo.m_usePlayerOffset = false;
	m_playerInfo.m_pos = math::Vector2( WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f );
	m_player2D->SetDrawInfo(m_playerInfo);

	// 描画クラスセットアップ
	m_life2D = NEW Game2DBase("lifeGauge.json");
	m_lifeInfo.Init();
	m_lifeInfo.m_prioity = PRIORITY_ABOVE_NORMAL;
	m_lifeInfo.m_usePlayerOffset = false;
	m_lifeInfo.m_pos.x = 0.0f;
	const TEX_INIT_INFO &lifeTexInfo = TextureResourceManager::GetInstance()->GetLoadTextureInfo( "lifeGauge.json" );
	m_lifeInfo.m_pos.y = static_cast<float>(WINDOW_HEIGHT - lifeTexInfo.m_sizeHeight);
	m_lifeInfo.m_arrangeOrigin = math::Vector2( 0.0f, 0.0f );
	m_life2D->SetDrawInfo(m_lifeInfo);

	m_lifeFrame2D = NEW Game2DBase("lifeFrame.json");
	m_lifeFrameInfo.Init();
	m_lifeFrameInfo.m_prioity = PRIORITY_HIGH;
	m_lifeFrameInfo.m_usePlayerOffset = false;
	m_lifeFrameInfo.m_pos = m_lifeInfo.m_pos;
	m_lifeFrameInfo.m_arrangeOrigin = m_lifeInfo.m_arrangeOrigin;
	m_lifeFrame2D->SetDrawInfo(m_lifeFrameInfo);

	// 攻撃マシンガンクラスセット
	m_attackGun = NEW AttackGun();
	SetChildUnit( m_attackGun );
}


GamePlayer::~GamePlayer(void)
{
}

void GamePlayer::Update()
{
	CallPadEvent();

	// 無敵時間中ならデクリメント
	if( m_invisibleTime > 0 ){
		--m_invisibleTime;
	}

	// 攻撃判定
	if( GetStickInfoRight().m_vec != math::Vector2() ){
		math::Vector2 pos = math::Vector2( m_playerInfo.m_pos.x, m_playerInfo.m_pos.y ) + GameAccesser::GetInstance()->GetPlayerOffSet();
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
		m_player2D->DrawUpdate2D();
	}

	// ライフゲージ描画
	{
		const TEX_INIT_INFO &lifeTexInfo = TextureResourceManager::GetInstance()->GetLoadTextureInfo( m_lifeInfo.m_fileName.c_str() );
		float ratio = static_cast<float>(m_playerLife)/static_cast<float>(LIFE_POINT_MAX);
		float lifeValue = (WINDOW_WIDTH / lifeTexInfo.m_sizeWidth) * ratio;
		
		if( math::Absf( m_lifeInfo.m_scale.x - lifeValue ) > 0.2f ){
			m_lifeInfo.m_scale.x *= (m_lifeInfo.m_scale.x - lifeValue < 0.0f) ? 1.02f : 0.98f ;
		}
		else{
			m_lifeInfo.m_scale.x = lifeValue;
		}

		m_life2D->SetDrawInfo(m_lifeInfo);
		m_life2D->DrawUpdate2D();
		m_lifeFrame2D->DrawUpdate2D();
	}
}

bool GamePlayer::DieMain(){
	SAFE_DELETE(m_life2D);
	SAFE_DELETE(m_lifeFrame2D);
	SAFE_DELETE(m_player2D);
	return true;
}

/* ================================================ */
/**
 * @brief	パッド操作関数
 */
/* ================================================ */
void GamePlayer::PadEventUp()
{
	GameAccesser::GetInstance()->AddPlayerOffSet(0.0f, -3.0f);

	if(IsButtonPush(InputWatcher::BUTTON_UP)){
		m_player2D->SetAnim("up");
	}
	else if(!IsButtonPress(InputWatcher::BUTTON_DOWN)
		&& !IsButtonPress(InputWatcher::BUTTON_RIGHT)
		&& !IsButtonPress(InputWatcher::BUTTON_LEFT)){
		m_player2D->SetAnim("up");
	}
	m_player2D->SetDrawInfo(m_playerInfo);
}

void GamePlayer::PadEventDown()
{
	GameAccesser::GetInstance()->AddPlayerOffSet(0.0f, 3.0f);

	if(IsButtonPush(InputWatcher::BUTTON_DOWN)){
		m_player2D->SetAnim("down");
	}
	else if(!IsButtonPress(InputWatcher::BUTTON_UP)
		&& !IsButtonPress(InputWatcher::BUTTON_RIGHT)
		&& !IsButtonPress(InputWatcher::BUTTON_LEFT)){
		m_player2D->SetAnim("down");
	}
	m_player2D->SetDrawInfo(m_playerInfo);
}

void GamePlayer::PadEventRight()
{
	GameAccesser::GetInstance()->AddPlayerOffSet(3.0f, 0.0f);

	if(IsButtonPush(InputWatcher::BUTTON_RIGHT)){
		m_player2D->SetAnim("right");
	}
	else if(!IsButtonPress(InputWatcher::BUTTON_DOWN)
		&& !IsButtonPress(InputWatcher::BUTTON_UP)
		&& !IsButtonPress(InputWatcher::BUTTON_LEFT)){
		m_player2D->SetAnim("right");
	}
	m_player2D->SetDrawInfo(m_playerInfo);
}

void GamePlayer::PadEventLeft()
{
	GameAccesser::GetInstance()->AddPlayerOffSet(-3.0f, 0.0f);

	if(IsButtonPush(InputWatcher::BUTTON_LEFT)){
		m_player2D->SetAnim("left");
	}
	else if(!IsButtonPress(InputWatcher::BUTTON_DOWN)
		&& !IsButtonPress(InputWatcher::BUTTON_RIGHT)
		&& !IsButtonPress(InputWatcher::BUTTON_UP)){
		m_player2D->SetAnim("left");
	}
	m_player2D->SetDrawInfo(m_playerInfo);
}

void GamePlayer::PadEventDecide()
{
	GameEffect *effect = NEW GameEffect( GameEffect::EFFECT_BOMB, 50,50 );
	GameRegister::GetInstance()->GetManagerItem()->CreateItem( ItemObject::ITEM_RAPID_BULLET );
}

void GamePlayer::PadEventCancel()
{
	GameRegister::GetInstance()->GetManagerEnemy()->CreateEnemy( Common::KIND_AAA );
}

/* ================================================ */
/**
 * @brief	情報取得関数
 */
/* ================================================ */
const TEX_DRAW_INFO &GamePlayer::GetDrawInfo()
{
	return m_player2D->GetDrawInfo();
}

/* ================================================ */
/**
 * @brief	タスクにイベント追加
 */
/* ================================================ */
void GamePlayer::AddEvent( const Common::CMN_EVENT &cmnEvent )
{
	TaskUnit::AddEvent( cmnEvent );
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
	if( m_playerLife > 20 ){
		m_playerLife -= 20;
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
	case ItemObject::ITEM_RAPID_BULLET:
		{
			// 銃の発射間隔を狭める
			AttackGun::GunState *gunState = m_attackGun->GetGunState();
			if( gunState ){
				gunState->m_shootInterval = ( gunState->m_shootInterval < 2 ) ? 0 : gunState->m_shootInterval - 2;
			}
		}
		break;
	}
}
