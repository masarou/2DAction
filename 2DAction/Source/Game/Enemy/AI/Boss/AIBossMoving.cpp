/* ====================================================================== */
/**
 * @brief  敵AIの通常クラス(プレイヤー未発見)
 *
 * @note
 *		デフォルトAI
 */
/* ====================================================================== */

#include "AIBossMoving.h"
#include "Game/Enemy/EnemyBoss.h"
#include "Common/Utility/CommonGameUtility.h"
#include "System/Draw2D/SystemDraw2DResource.h"

AIBossMoving *AIBossMoving::Create()
{
	AIBossMoving *tmpAI = NEW AIBossMoving();
	return tmpAI;
}

AIBossMoving::AIBossMoving(void)
{
	m_targetPlace = EnemyBoss::BOSS_TARGET_PLACE;
}


AIBossMoving::~AIBossMoving(void)
{
}

bool AIBossMoving::InitAI()
{
	return true;
}

void AIBossMoving::ExecMain( TEX_DRAW_INFO &enemyInfo, ACTION_ARRAY &actionInfo )
{
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


bool AIBossMoving::SearchPlayer( TEX_DRAW_INFO &enemyInfo )
{
	bool retVal = false;
	if( math::IsInRange( Utility::GetPlayerPos(), enemyInfo.m_posOrigin, 300.0f ) ){
		retVal = true;
	}
	return retVal;
}