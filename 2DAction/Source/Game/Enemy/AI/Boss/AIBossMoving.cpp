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
}


bool AIBossMoving::SearchPlayer( TEX_DRAW_INFO &enemyInfo )
{
	bool retVal = false;
	if( math::IsInRange( Utility::GetPlayerPos(), enemyInfo.m_posOrigin, 300.0f ) ){
		retVal = true;
	}
	return retVal;
}