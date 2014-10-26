

#include "GamePlayer.h"
#include "Game/Effect/GameEffect.h"

#include "System/Sound/SystemSoundManager.h"
#include "Game/Enemy/EnemyManager.h"
#include "Game/GameRegister.h"

static uint32_t DAMAGE_INVISIBLE_TIME = 180;

GamePlayer *GamePlayer::CreatePlayer()
{
	return NEW GamePlayer();
}

GamePlayer::GamePlayer(void)
: TaskUnit("Player")
, m_invisibleTime(0)
{
	// 描画クラスセットアップ
	m_player2D = NEW Game2DBase("player.json");
	m_playerInfo.Init();
	m_playerInfo.m_prioity = PRIORITY_ABOVE_NORMAL;
	m_playerInfo.m_usePlayerOffset = false;
	m_playerInfo.m_pos.x = WINDOW_WIDTH / 2.0f;
	m_playerInfo.m_pos.y = WINDOW_HEIGHT / 2.0f;
	m_player2D->SetDrawInfo(m_playerInfo);

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
#ifdef _DEBUG
	int Color = GetColor( 255 , 255 , 255 );
	float xx = 0.0f;
	float yy = 0.0f;
	GameAccesser::GetInstance()->GetPlayerOffSet(xx, yy);
	DrawFormatString( 0, 10, Color, "PlayerX = %.1f, PlayerY = %.1f\n", xx, yy);
#endif

	if( m_invisibleTime % 3 == 1 ){
		// ダメージを受けない時間帯ならば3フレに一回描画せず点滅させる
	}
	else{
		// プレイヤー描画
		m_player2D->DrawUpdate2D();
	}
}

bool GamePlayer::DieMain(){
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
	//SoundManager::GetInstance()->PlayBGM("SE_TEST03");
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
	//SoundManager::GetInstance()->PlayBGM("SE_TEST04");
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
