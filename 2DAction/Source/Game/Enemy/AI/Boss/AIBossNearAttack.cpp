/* ====================================================================== */
/**
 * @brief  敵AIの通常クラス(プレイヤー未発見)
 *
 * @note
 *		デフォルトAI
 */
/* ====================================================================== */

#include "AIBossNearAttack.h"
#include "Game/Enemy/EnemyBoss.h"
#include "Common/Utility/CommonGameUtility.h"
#include "System/Draw2D/SystemDraw2DResource.h"

AIBossNearAttack *AIBossNearAttack::Create()
{
	AIBossNearAttack *tmpAI = NEW AIBossNearAttack();
	return tmpAI;
}

AIBossNearAttack::AIBossNearAttack(void)
: m_currActionTime( 0 )
, m_targetPlace( math::Vector2() )
, m_actionType( ACTION_MAX )
{
}


AIBossNearAttack::~AIBossNearAttack(void)
{
}

bool AIBossNearAttack::InitAI()
{
	//if( Utility::GetRandamValue( 1, 0 ) == 1 ){
		// ショットガン
		ChangeActionType( ACTION_SPREAD_BULLET );
	//}
	//else{
		// 斬撃
		ChangeActionType( ACTION_SLASHING );
	//}

	return true;
}

void AIBossNearAttack::ExecMain( TEX_DRAW_INFO &enemyInfo, ACTION_ARRAY &actionInfo )
{
	if( !math::IsInRange( enemyInfo.m_posOrigin, Utility::GetPlayerPos(), DISTANCE_TO_PLAYER_FAR ) ){
		// 一定距離離れた
		if( Utility::GetRandamValue( 60, 0 ) == 0 ){
			GameEffect::CreateEffect( GameEffect::EFFECT_PRE_EXPLOSION, Utility::GetPlayerPos() );
		}
		return;
	}

	// 攻撃中なら行動しない
	if( s_pAttackBlade->IsSlashingAnimPlay() ){
		return;
	}

	if( m_targetPlace != math::Vector2() ){
		// もし、目的地がセットされていたらそこまで移動してから以下の行動関数を実行する
		math::Vector2 moveVec = m_targetPlace - enemyInfo.m_posOrigin;
		if( moveVec.GetLengthSquare() < 10.0f*10.0f ){
			// 移動終了
			m_targetPlace = math::Vector2();
		}
		else{
			moveVec *= 0.075f;
		}
		if( Utility::GetMapHeight( enemyInfo.m_posOrigin + moveVec ) == 0 ){
			enemyInfo.m_posOrigin += moveVec;
		}
		else{
			// 移動終了
			m_targetPlace = math::Vector2();
		}
	}
	else{
		// 行動関数をセット
		switch( m_actionType ){
		case ACTION_SPREAD_BULLET:
			ExecSpreadBullet( enemyInfo, actionInfo );
			break;
		case ACTION_SLASHING:
			ExecSlashing( enemyInfo, actionInfo );
			break;
		}
	}

	// 行動関数監視、終了したら次の行動へ

	// 繰り返し

	// 常にプレイヤーとの距離を測り一定以上離れたら状態変更



	// アニメ更新
	math::Vector2 vec = GetEnemyEyeSight();
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
	SetEnemyEyeSight( vec );

	// プレイヤーに向かって弾を発射
	if( Utility::GetRandamValue( 120, 0 ) == 0 ){
		ShootBullet();
	}
}

void AIBossNearAttack::ChangeActionType( const ACTION_TYPE &nextType )
{
	m_actionType = nextType;
	m_currActionTime = GetNowCount();
}

bool AIBossNearAttack::ExecSpreadBullet( TEX_DRAW_INFO &enemyInfo, ACTION_ARRAY &actionInfo )
{
	if( Utility::GetRandamValue( 30, 0 ) == 0 ){
		// スプレッド弾発射
		for( uint32_t i = 0; i < 10 ; ++i ){
			// 自キャラとプレイヤーがなす線と左右±30.0度の範囲に発射
			math::Vector2 myPos = enemyInfo.m_posOrigin;
			math::Vector2 plPos = Utility::GetPlayerPos();

			int32_t randamValue	= Utility::GetRandamValue( 30, -30 );
			uint32_t speed		= Utility::GetRandamValue( 20, 10 );
			math::Vector2 dir = math::GetRotateVec( plPos - myPos, static_cast<float>(randamValue) );
			ShootBullet( dir, 20, speed );
		}

		if( Utility::GetRandamValue( 3, 0 ) == 0 ){
			// 一定確率で行動変化
			ChangeActionType( ACTION_SLASHING );
		}
		else{
			// また同じスプレッド攻撃
			ChangeActionType( ACTION_SPREAD_BULLET );
		}
	}
	return true;
}

bool AIBossNearAttack::ExecSlashing( TEX_DRAW_INFO &enemyInfo, ACTION_ARRAY &actionInfo )
{
	// 
	uint32_t currTime = GetNowCount();

	// 自分とプレイヤーの距離によって行動を分ける
	if( !math::IsInRange( enemyInfo.m_posOrigin, Utility::GetPlayerPos(), 100.0f )
		&& currTime - m_currActionTime < 3000/*3sec*/ ){
		// まだ距離が遠いので接近
		math::Vector2 moveVec	= Utility::GetPlayerPos() - enemyInfo.m_posOrigin;
		math::Vector2 addvec	= math::Vector2();
		if( moveVec.GetLengthSquare() < 10.0f*10.0f ){
			// 一定以上近づいているなら一気に縮める
			addvec += moveVec;
		}
		else{
			addvec += moveVec * 0.075f;
		}
		if( Utility::GetMapHeight( enemyInfo.m_posOrigin + addvec ) == 0 ){
			enemyInfo.m_posOrigin += addvec;
		}
		SetEnemyEyeSight( moveVec );
	}
	else{
		// 斬撃攻撃
		Slashing( Slashing::TYPE_3RD );

		// 攻撃し終えたら離れる
		// 移動先を決定
		math::Vector2 nextTargetVec = GetEnemyEyeSight() * -1.0f;
		int32_t randamValue		= Utility::GetRandamValue( 160, -160 );
		int32_t awayDistance	= Utility::GetRandamValue( 300, 700 );

		// 次の目的地をいい感じに求める
		nextTargetVec = math::GetRotateVec( nextTargetVec, static_cast<float>(randamValue) );

		// 目的地に視線をセット
		SetEnemyEyeSight( nextTargetVec );

		nextTargetVec.Normalize();
		nextTargetVec *= static_cast<float>(awayDistance);
		nextTargetVec += enemyInfo.m_posOrigin;

		// 目的地をセット
		m_targetPlace = nextTargetVec;

		// 斬撃の行動終了 次の行動へ
		if( Utility::GetRandamValue( 2, 0 ) == 0 ){
			// 一定確率で行動変化
			ChangeActionType( ACTION_SPREAD_BULLET );
		}
		else{
			// まだ同じ斬撃行動へ
			ChangeActionType( ACTION_SLASHING );
		}
	}

	return true;
}