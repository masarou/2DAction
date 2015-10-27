/* ====================================================================== */
/**
 * @brief  敵AIの通常クラス(プレイヤー未発見)
 *
 * @note
 *		デフォルトAI
 */
/* ====================================================================== */

#include "Game/GameRegister.h"
#include "Game/Enemy/EnemyManager.h"
#include "Game/Enemy/EnemyBase.h"
#include "Game/Enemy/EnemyWizard.h"

#include "EnemyAIDragon.h"
#include "Game/Effect/GameEffect.h"
#include "Common/Utility/CommonGameUtility.h"
#include "System/Sound/SystemSoundManager.h"
#include "System/Draw2D/SystemDraw2DResource.h"
#include "System/SystemFPSManager.h"

AIDragon *AIDragon::Create()
{
	AIDragon *tmpAI = NEW AIDragon();
	return tmpAI;
}

AIDragon::AIDragon(void)
: m_movingPos( math::Vector2() )
, m_currAction( ACTION_MAX )
, m_nextAction( ACTION_MAX )
, m_waitCounter( 0 )
{
}


AIDragon::~AIDragon(void)
{
}

bool AIDragon::InitAI()
{
	m_currAction = ACTION_FIRE_BOMB;
	m_nextAction = ACTION_MAX;

	// 最初の待ち時間セット
	m_waitCounter = ACTION_INTERVAL_SHORT;
	return true;
}

void AIDragon::ExecMain( TEX_DRAW_INFO &enemyInfo, ACTION_ARRAY &actionInfo )
{
	// 以下、攻撃関数
	if( m_waitCounter > 0 ){
		--m_waitCounter;
		return;
	}

	bool isFinish = false;
	switch( m_currAction ){
	default:
	case ACTION_FIRE_BOMB:
		isFinish = ActionShootFireBall( enemyInfo );
		break;
	case ACTION_MOVE_AWAY:
		isFinish = ActionAwayFromPlayer( enemyInfo );
		break;
	case ACTION_LARGE_EXPLOSION:
		isFinish = ActionFireWall( enemyInfo );
		break;
	}

	// コルーチンリセット
	if( isFinish ){
		coroutine tmp;
		m_coro = tmp;

		// 次のアクションがセットされていれば変更
		if( m_nextAction != ACTION_MAX ){
			m_currAction = m_nextAction;
			m_nextAction = ACTION_MAX;
		}
	}

	// アニメ更新
	std::string animTag = "";
	math::Vector2 vec = Utility::GetPlayerPos() - enemyInfo.m_posOrigin;
	vec.Normalize();
	switch( Utility::GetDirection( vec.x, vec.y ) ){
	default:
		break;
	case InputWatcher::BUTTON_UP:
		animTag = "up";
		break;
	case InputWatcher::BUTTON_DOWN:
		animTag = "down";
		break;
	case InputWatcher::BUTTON_LEFT:
		animTag = "left";
		break;
	case InputWatcher::BUTTON_RIGHT:
		animTag = "right";
		break;
	}
	SetEnemyAnim( animTag );
	SetEnemyEyeSight( vec );

	// 移動
	enemyInfo.m_posOrigin += vec * static_cast<float>( GetEnemySPD() );
}

/* ================================================ */
/**
 * @brief	敵クラスが他のクラスからもらったイベント
 */
/* ================================================ */
void AIDragon::EnemyRecievedEvent( const Common::CMN_EVENT &eventInfo )
{
	if( GetEnemeyMine()->GetStatus() != TaskUnit::TASK_ALIVE ){
		return;
	}

	switch( eventInfo.m_event ){
	case Common::EVENT_HIT_BULLET_PLAYER:	// Playerの弾に当たった
		break;

	case Common::EVENT_HIT_BLADE_PLAYER:	// Playerの斬撃に当たった
		SetInterruptAction( ACTION_MOVE_AWAY );
		break;
	}
}

/* ================================================ */
/**
 * @brief	次の行動定義取得
 */
/* ================================================ */
AIDragon::ACTION_TYPE AIDragon::GetRandamNextAction()
{
	uint32_t type = Utility::GetRandamValue( ACTION_MAX, 0 );
	return static_cast<ACTION_TYPE>( type );
}

/* ================================================ */
/**
 * @brief	行動タイプ変更
 */
/* ================================================ */
void AIDragon::ChangeActionType( const ACTION_TYPE &nextAction )
{
	if( m_currAction == ACTION_MAX ){
		m_currAction = nextAction;
	}
	else{
		m_nextAction = nextAction;
	}
}
void AIDragon::SetInterruptAction( const ACTION_TYPE &interruptAction )
{
	m_waitCounter = 0;
	m_currAction = interruptAction;
}


/* ================================================ */
/**
 * @brief	火の玉発射
 */
/* ================================================ */
bool AIDragon::ActionShootFireBall( TEX_DRAW_INFO &enemyInfo, const bool &onlyAction )
{
	math::Vector2 vec = Utility::GetPlayerPos() - enemyInfo.m_posOrigin;
	vec.Normalize();

	// 火の玉クラス作成 + SE再生
	DragonFireBomb::Create( enemyInfo.m_posOrigin, vec, 20 + ( GetEnemyLevel() * 3 ) );
	SoundManager::GetInstance()->PlaySE("FireBall");

	if( !onlyAction ){
		// 次の行動セット
		m_waitCounter = ACTION_INTERVAL;
		ChangeActionType( GetRandamNextAction() );
	}

	return true;
}

/* ================================================ */
/**
 * @brief	プレイヤーから距離を取る時の目的地設定
 */
/* ================================================ */
bool AIDragon::SetRunAwayPosFromPlayer( TEX_DRAW_INFO &enemyInfo )
{
	// 移動先を決定
	math::Vector2 nextTargetVec = GetEnemyEyeSight() * -1.0f;
	for( uint32_t i = 1;;++i ){
		if( i > 100 ){
			DEBUG_ASSERT( 0, "目標となる位置がない\n");
			break;
		}
		int32_t awayAngle		= 0;
		if( i < 10 ){
			awayAngle = Utility::GetRandamValue( 60, 60 );
		}
		else{
			awayAngle = Utility::GetRandamValue( i*5, i*-5 );
		}
		int32_t awayDistance	= Utility::GetRandamValue( 300, 800 );
		
		// 次の目的地を求める
		math::Vector2 moveVec		= math::GetRotateVec( nextTargetVec, static_cast<float>(awayAngle) );
		moveVec.Normalize();
		math::Vector2 candidatePos	= enemyInfo.m_posOrigin + (moveVec * static_cast<float>(awayDistance) );
		if( Utility::GetMapHeight( candidatePos ) == 0 ){
			// 目的地決定
			nextTargetVec = candidatePos;
			break;
		}
	}

	// 目的地に視線をセット
	SetEnemyEyeSight( nextTargetVec );

	// 目的地をセット
	m_movingPos = nextTargetVec;

	return true;
}

/* ================================================ */
/**
 * @brief	設定された目的地に移動 trueで到達
 */
/* ================================================ */
bool AIDragon::DashMove( TEX_DRAW_INFO &enemyInfo, const math::Vector2 &targetPos )
{
	bool retVal = false;
	if( !math::IsInRange( enemyInfo.m_posOrigin, targetPos, 100.0f ) ){
		// まだ距離が遠いので接近
		math::Vector2 moveVec	= targetPos - enemyInfo.m_posOrigin;
		math::Vector2 addvec	= math::Vector2();
		if( moveVec.GetLengthSquare() < 10.0f*10.0f ){
			// 一定以上近づいているなら一気に縮める
			addvec += moveVec;
		}
		else{
			addvec += moveVec * 0.02f;
		}

		// 一定速度を超えていたら速度を下げる
		if( addvec.GetLengthSquare() > 3.0f*3.0f ){
			addvec.Normalize();
			addvec *= 3.0f*3.0f;
		}

		if( Utility::GetMapHeight( enemyInfo.m_posOrigin + addvec ) == 0 ){
			enemyInfo.m_posOrigin += addvec;
			SetEnemyEyeSight( moveVec );
		}
		else{
			// 移動先が進行出来ないなら次のアクションへ
			retVal = true;
		}
	}
	else{
		retVal = true;
	}

	// 移動しながら火炎弾を吐く
	if( FpsManager::GetUpdateCounter() % 30 == 0 ){
		ActionShootFireBall( enemyInfo, /*bool onlyAction=*/true );
	}

	return retVal;
}







DragonFireBomb *DragonFireBomb::Create( const math::Vector2 &pos, const math::Vector2 &vec, const uint32_t &damage )
{
	return NEW DragonFireBomb( pos, vec, damage );
}

DragonFireBomb::DragonFireBomb( const math::Vector2 &pos, const math::Vector2 &vec, const uint32_t &damage )
: TaskUnit( "Fire" )
, m_liveTime( 0 )
, m_liveTimeMax( 0 )
, m_vec( vec )
, m_damage( damage )
{
	// 描画クラスの準備
	std::string jsonStr = "FireBall.json";
	m_drawTexture.m_pTex2D = Game2DBase::Create( jsonStr.c_str() );

	//!初期位置セット
	TEX_DRAW_INFO drawInfo;
	drawInfo.Init();
	drawInfo.m_fileName = jsonStr;
	drawInfo.m_posOrigin = pos;
	drawInfo.m_rot = pos.GetAngle( Utility::GetPlayerPos() );
	m_drawTexture.m_pTex2D->SetDrawInfo( drawInfo );

	m_liveTimeMax = 300;
}

DragonFireBomb::~DragonFireBomb()
{


}

bool DragonFireBomb::DieMain()
{
	return true;
}

bool DragonFireBomb::Init()
{
	return true;
}

/* ================================================ */
/**
 * @brief	各種Update関数
 */
/* ================================================ */
void DragonFireBomb::Update()
{
	std::string currAnim = "move";
	m_drawTexture.m_pTex2D->UpdateDrawInfo().m_posOrigin += m_vec * 12.0f;
	++m_liveTime;

	if( m_liveTime >= m_liveTimeMax ){
		// 生成から一定時間たったなら自殺
		TaskStartDie();
	}
}

void DragonFireBomb::DrawUpdate()
{
	// 弾描画
	m_drawTexture.m_pTex2D->DrawUpdate2D();
}

/* ================================================ */
/**
 * @brief	他クラスからのイベント処理
 */
/* ================================================ */
void DragonFireBomb::EventUpdate( Common::CMN_EVENT &eventId )
{
	switch( eventId.m_event ){
	default:

		break;
		
	case Common::EVENT_HIT_PLAYER:
		// このクラスは死亡 + 炎エフェクトを作成
		m_liveTime = m_liveTimeMax;
		TaskStartDie();
		GameEffectWithCollision::CreateEffect( Common::OWNER_ENEMY, GameEffectWithCollision::EFFECT_FIRE, Utility::GetPlayerPos() );
		break;

	case Common::EVENT_HIT_BLADE_PLAYER:
		break;
	}
}

const Common::TYPE_OBJECT DragonFireBomb::GetTypeObject() const
{
	return Common::TYPE_FIRE_BALL;
}


