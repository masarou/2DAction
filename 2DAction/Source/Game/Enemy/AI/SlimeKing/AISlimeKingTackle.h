/* ====================================================================== */
/**
 * @brief  スライムキングの攻撃クラス(近距離)
 *
 * @note
 *		攻撃AI
 */
/* ====================================================================== */

#ifndef __ENEMY_AI_SLIME_KING_TACKLE__
#define __ENEMY_AI_SLIME_KING_TACKLE__

#include "Game/Enemy/AI/EnemyAIBase.h"

class AISlimeKingTackle : public EnemyAIBase
{

public:

	static AISlimeKingTackle *Create();

	// AIの種類を派生先でセットしておく
	virtual const Common::ENEMY_AI GetAIKind() const{ return Common::AI_MOVE_PLAYER_SLIME_KING; }

protected:
	
	virtual bool InitAI() override;								// AI初期化
	virtual void ExecMain( TEX_DRAW_INFO &enemyInfo, ACTION_ARRAY &actionInfo ) override;	// 派生先でのAI実装

private:

	AISlimeKingTackle(void);
	~AISlimeKingTackle(void);

};
#endif