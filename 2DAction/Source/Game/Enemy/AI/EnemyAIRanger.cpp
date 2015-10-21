/* ====================================================================== */
/**
 * @brief RangerAIクラス
 *
 * @note
 *		デフォルトAI
 */
/* ====================================================================== */

#include "EnemyAIRanger.h"
#include "Game/Enemy/EnemyRanger.h"
#include "Common/Utility/CommonGameUtility.h"
#include "System/Draw2D/SystemDraw2DResource.h"

AIRanger *AIRanger::Create()
{
	AIRanger *tmpAI = NEW AIRanger();
	return tmpAI;
}

AIRanger::AIRanger(void)
: m_actionIntervalTime( 0 )
, m_currActionType( ACTION_MAX )
, m_nextActionType( ACTION_MAX )
, m_currActionTime( 0 )
, m_movingPos( math::Vector2() )
{
}


AIRanger::~AIRanger(void)
{
}

bool AIRanger::InitAI()
{
	m_actionIntervalTime = ACTION_INTERVAL_SHORT;

	// 斬撃
	ChangeActionType( ACTION_SLASHING );

	return true;
}

void AIRanger::ExecMain( TEX_DRAW_INFO &enemyInfo, ACTION_ARRAY &actionInfo )
{
	// 攻撃中なら行動しない
	if( s_pAttackBlade->IsSlashingAnimPlay() ){
		return;
	}

	// 行動直後なのでなにもしない
	if( m_actionIntervalTime > 0 ){
		--m_actionIntervalTime;
		return;
	}

	bool isActionEnd = true;
	switch( m_currActionType ){
	case ACTION_SPREAD_BULLET:
		isActionEnd = ExecSpreadBullet( enemyInfo );
		break;
	case ACTION_SLASHING:
		isActionEnd = ExecSlashing( enemyInfo );
		break;
	case ACTION_MOVE:
		isActionEnd = ExecMoving( enemyInfo );
		break;
	}

	// コルーチンリセット
	if( isActionEnd ){
		coroutine tmp;
		m_coro = tmp;

		// 次のアクションがセットされていれば変更
		if( m_nextActionType != ACTION_MAX ){
			m_currActionType = m_nextActionType;
			m_nextActionType = ACTION_MAX;
		}
	}

	// アニメ更新
	math::Vector2 vec = Utility::GetPlayerPos() - enemyInfo.m_posOrigin;
	std::string animTag = "";
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
}

void AIRanger::ChangeActionType( const ACTION_TYPE &nextType )
{
	if( m_currActionType == ACTION_MAX ){
		m_currActionType = nextType;
	}
	else{
		m_nextActionType = nextType;
	}
}

AIRanger::ACTION_TYPE AIRanger::GetNextAction()
{
	ACTION_TYPE type;
	
	for(;;){
		// スプレッドの場合相手に近すぎないかチェック
		type = static_cast< ACTION_TYPE >( Utility::GetRandamValue( static_cast<uint32_t>(ACTION_MAX), 0 ) );
		if( type == ACTION_SPREAD_BULLET && math::IsInRange( Utility::GetPlayerPos(), GetEnemeyMine()->GetDrawInfo().m_posOrigin, 300.0f ) ){
			continue;
		}
		else{
			break;
		}
	}
	return type;
}

bool AIRanger::ExecSpreadBullet( TEX_DRAW_INFO &enemyInfo )
{
	// スプレッド弾発射
	for( uint32_t i = 0; i < 8 ; ++i ){
		// 自キャラとプレイヤーがなす線と左右±25.0度の範囲に発射
		math::Vector2 myPos = enemyInfo.m_posOrigin;
		math::Vector2 plPos = Utility::GetPlayerPos();

		int32_t randamValue	= Utility::GetRandamValue( 25, -25 );
		uint32_t speed		= Utility::GetRandamValue( 18, 13 );
		math::Vector2 dir = math::GetRotateVec( plPos - myPos, static_cast<float>(randamValue) );
		ShootBullet( enemyInfo.m_posOrigin, dir, 7*GetEnemyLevel(), speed );
	}

	// 次の行動セット
	ChangeActionType( GetNextAction() );

	m_actionIntervalTime = Utility::GetRandamValue( ACTION_INTERVAL, ACTION_INTERVAL_SHORT );
	return true;
}

bool AIRanger::DashMove( TEX_DRAW_INFO &enemyInfo, const math::Vector2 &targetPos )
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
			addvec += moveVec * 0.055f;
		}

		// 一定速度を超えていたら速度を下げる
		if( addvec.GetLengthSquare() > 5.0f*5.0f ){
			addvec.Normalize();
			addvec *= 5.0f*5.0f;
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
	return retVal;
}

bool AIRanger::SetRunAwayPosFromPlayer( TEX_DRAW_INFO &enemyInfo )
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

