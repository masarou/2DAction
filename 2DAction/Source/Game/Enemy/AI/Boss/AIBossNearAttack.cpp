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
{
	m_targetPlace = EnemyBoss::BOSS_TARGET_PLACE;
}


AIBossNearAttack::~AIBossNearAttack(void)
{
}

bool AIBossNearAttack::InitAI()
{
	if( Utility::GetRandamValue( 1, 0 ) == 0 ){
		// ショットガン
		m_actionType = ACTION_SPREAD_BULLET;
	}
	else{
		// 斬撃
		m_actionType = ACTION_SLASHING;
	}

	return true;
}

void AIBossNearAttack::ExecMain( TEX_DRAW_INFO &enemyInfo, ACTION_ARRAY &actionInfo )
{
	if( !math::IsInRange( enemyInfo.m_posOrigin, Utility::GetPlayerPos(), DISTANCE_TO_PLAYER_FAR ) ){
		// 一定距離離れた
		//ChangeEnemyAI( Common::AI_CREATE_ENEMY
		return;
	}

	// 行動関数をセット
	switch( m_actionType ){
	case ACTION_SPREAD_BULLET:
		ExecSpreadBullet( enemyInfo, actionInfo );
		break;
	case ACTION_SLASHING:
		ExecSlashing( enemyInfo, actionInfo );
		break;
	case ACTION_RUN_AWAY:
		ExecRunAway( enemyInfo, actionInfo );
		break;
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
		// 行動の詳細をセット
		ACTION_INFO action;
		action.m_AItype = AI_SHOOT_BULLET;
		action.m_AIInfo.Init();
		action.m_EventMessage	= Common::EVENT_SHOOT_BULLET;

		// 後ほどイベントメッセージとして投げてもらう
		actionInfo.m_pushEventArray.push_back( action );
	}
}


bool AIBossNearAttack::ExecSpreadBullet( TEX_DRAW_INFO &enemyInfo, ACTION_ARRAY &actionInfo )
{
	if( math::IsInRange( enemyInfo.m_posOrigin, Utility::GetPlayerPos(), 150.0f ) ){

		// スプレッド弾発射
		//CreateSpreadBullet( actionInfo );
		//actionInfo.m_pushEventArray.push_back( Common::EVENT_SHOOT_BULLET );
	}
	return true;
}

bool AIBossNearAttack::ExecSlashing( TEX_DRAW_INFO &enemyInfo, ACTION_ARRAY &actionInfo )
{
	return true;
}

bool AIBossNearAttack::ExecRunAway( TEX_DRAW_INFO &enemyInfo, ACTION_ARRAY &actionInfo )
{
	return true;
}

void AIBossNearAttack::CreateSpreadBullet( ACTION_ARRAY &actionInfo )
{
	for( uint32_t i = 0; i < 10 ; ++i ){

	}
}

