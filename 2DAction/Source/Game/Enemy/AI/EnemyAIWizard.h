/* ====================================================================== */
/**
 * @brief  敵WizardクラスAI
 *
 * @note
 *		デフォルトAI
 */
/* ====================================================================== */

#ifndef __ENEMY_AI_WIZARD__
#define __ENEMY_AI_WIZARD__

#include "EnemyAIBase.h"

class AIWizard : public EnemyAIBase
{
public:

	static AIWizard *Create();

	// AIの種類を派生先でセットしておく
	virtual const Common::ENEMY_AI GetAIKind() const{ return Common::AI_ATTACK_WIZARD; }

protected:
	
	virtual bool InitAI() override;								// AI初期化
	virtual void ExecMain( TEX_DRAW_INFO &enemyInfo, ACTION_ARRAY &actionInfo ) override;	// 派生先でのAI実装

private:

	bool SearchPlayer( TEX_DRAW_INFO &enemyInfo, float distance = 300.0f );				// プレイヤー探索

	AIWizard(void);
	~AIWizard(void);

	uint32_t	m_circleRadius;		// 徘徊用の円の大きさ(半径)
	uint32_t	m_circleDistance;	// 自身の地点から徘徊円までの距離

};


#endif