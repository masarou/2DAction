/* ====================================================================== */
/**
 * @brief  スライムキングAIの通常クラス(プレイヤー未発見)
 *
 * @note
 *		デフォルトAI
 */
/* ====================================================================== */

#ifndef __ENEMY_AI_KING_SLIME_SEARCHING__
#define __ENEMY_AI_KING_SLIME_SEARCHING__

#include "Game/Enemy/AI/EnemyAIBase.h"

class AISlimeKingSearching : public EnemyAIBase
{
public:

	static AISlimeKingSearching *Create();

	// AIの種類を派生先でセットしておく
	virtual const Common::ENEMY_AI GetAIKind() const{ return Common::AI_SEARCHING_SLIME_KING; }

protected:
	
	virtual bool InitAI() override;								// AI初期化
	virtual void ExecMain( TEX_DRAW_INFO &enemyInfo, ACTION_ARRAY &actionInfo ) override;	// 派生先でのAI実装

private:

	bool SearchPlayer( TEX_DRAW_INFO &enemyInfo, float distance = 300.0f );				// プレイヤー探索

	AISlimeKingSearching(void);
	~AISlimeKingSearching(void);

	uint32_t	m_circleRadius;		// 徘徊用の円の大きさ(半径)
	uint32_t	m_circleDistance;	// 自身の地点から徘徊円までの距離

};

#endif