/* ====================================================================== */
/**
 * @brief  ラスボス両手AIクラス
 *
 * @note
 *		デフォルトAI
 */
/* ====================================================================== */

#ifndef __ENEMY_AI_LAST_BOSS_HAND__
#define __ENEMY_AI_LAST_BOSS_HAND__

#include "Game/Enemy/AI/EnemyAIBase.h"

class EnemyAILastBossHand : public EnemyAIBase
{
public:

	static EnemyAILastBossHand *Create();

	// AIの種類を派生先でセットしておく
	virtual const Common::ENEMY_AI GetAIKind() const{ return Common::AI_ATTACK_LAST_BOSS_HAND; }

protected:
	
	virtual bool InitAI() override;								// AI初期化
	virtual void ExecMain( TEX_DRAW_INFO &enemyInfo, ACTION_ARRAY &actionInfo ) override;	// 派生先でのAI実装

private:

	EnemyAILastBossHand(void);
	~EnemyAILastBossHand(void);

};

#endif