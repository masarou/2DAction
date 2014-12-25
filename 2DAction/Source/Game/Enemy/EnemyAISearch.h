/* ====================================================================== */
/**
 * @brief  敵AIの通常クラス(プレイヤー未発見)
 *
 * @note
 *		デフォルトAI
 */
/* ====================================================================== */

#ifndef ENEMY_AI_SEARCH
#define ENEMY_AI_SEARCH

#include "EnemyAIBase.h"

class EnemyAISearch : public EnemyAIBase
{
public:

	static EnemyAISearch *Create();

	// AIの種類を派生先でセットしておく
	virtual const Common::ENEMY_AI GetAIKind() const{ return Common::AI_SEARCHING; }

protected:
	
	virtual bool InitAI() override{ return true; }					// AI初期化
	virtual void ExecMain( TEX_DRAW_INFO &enemyInfo ) override{};	// 派生先でのAI実装

private:

	EnemyAISearch(void);
	~EnemyAISearch(void);

};

#endif