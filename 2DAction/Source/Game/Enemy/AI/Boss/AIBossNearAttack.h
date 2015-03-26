/* ====================================================================== */
/**
 * @brief  敵AIの通常クラス(プレイヤー未発見)
 *
 * @note
 *		デフォルトAI
 */
/* ====================================================================== */

#ifndef __AI_BOSS_NEAE_ATTACK__
#define __AI_BOSS_NEAE_ATTACK__

#include "Game/Enemy/AI/EnemyAIBase.h"

class AIBossNearAttack : public EnemyAIBase
{
public:

	static AIBossNearAttack *Create();

	// AIの種類を派生先でセットしておく
	virtual const Common::ENEMY_AI GetAIKind() const{ return Common::AI_SEARCHING; }

protected:
	
	virtual bool InitAI() override;								// AI初期化
	virtual void ExecMain( TEX_DRAW_INFO &enemyInfo, ACTION_ARRAY &actionInfo ) override;	// 派生先でのAI実装

private:

	enum ACTION_TYPE{
		ACTION_SPREAD_BULLET,
		ACTION_SLASHING,
		ACTION_RUN_AWAY,

		ACTION_MAX,
	};

	bool ExecSpreadBullet( TEX_DRAW_INFO &enemyInfo, ACTION_ARRAY &actionInfo );	// ショットガン的な攻撃
	bool ExecSlashing( TEX_DRAW_INFO &enemyInfo, ACTION_ARRAY &actionInfo );		// 斬撃攻撃
	bool ExecRunAway( TEX_DRAW_INFO &enemyInfo, ACTION_ARRAY &actionInfo );		// 距離をとる

	// 攻撃関数
	void CreateSpreadBullet( ACTION_ARRAY &actionInfo );

	AIBossNearAttack(void);
	~AIBossNearAttack(void);

	math::Vector2	m_targetPlace;
	ACTION_TYPE		m_actionType;
};

#endif