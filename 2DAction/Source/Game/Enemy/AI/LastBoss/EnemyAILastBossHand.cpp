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

#include "EnemyAILastBossHand.h"
#include "Common/Utility/CommonGameUtility.h"
#include "System/Draw2D/SystemDraw2DResource.h"
#include "Game/Effect/GameEffect.h"

EnemyAILastBossHand *EnemyAILastBossHand::Create()
{
	EnemyAILastBossHand *tmpAI = NEW EnemyAILastBossHand();
	return tmpAI;
}

EnemyAILastBossHand::EnemyAILastBossHand(void)
{
}


EnemyAILastBossHand::~EnemyAILastBossHand(void)
{
}

bool EnemyAILastBossHand::InitAI()
{
	return true;
}

void EnemyAILastBossHand::ExecMain( TEX_DRAW_INFO &enemyInfo, ACTION_ARRAY &actionInfo )
{

}

