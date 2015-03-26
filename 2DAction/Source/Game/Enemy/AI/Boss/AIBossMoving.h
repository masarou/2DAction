/* ====================================================================== */
/**
 * @brief  敵AIの通常クラス(プレイヤー未発見)
 *
 * @note
 *		デフォルトAI
 */
/* ====================================================================== */

#ifndef __AI_BOSS_MOVING__
#define __AI_BOSS_MOVING__

#include "Game/Enemy/AI/EnemyAIBase.h"

class AIBossMoving : public EnemyAIBase
{
public:

	static AIBossMoving *Create();

	// AIの種類を派生先でセットしておく
	virtual const Common::ENEMY_AI GetAIKind() const{ return Common::AI_SEARCHING; }

protected:
	
	virtual bool InitAI() override;								// AI初期化
	virtual void ExecMain( TEX_DRAW_INFO &enemyInfo, ACTION_ARRAY &actionInfo ) override;	// 派生先でのAI実装

private:

	bool SearchPlayer( TEX_DRAW_INFO &enemyInfo );				// プレイヤー探索

	AIBossMoving(void);
	~AIBossMoving(void);

	math::Vector2	m_targetPlace;
};

#endif