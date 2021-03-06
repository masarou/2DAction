/* ====================================================================== */
/**
 * @brief  GAIÌUNX(ß£)
 *
 * @note
 *		UAI
 */
/* ====================================================================== */

#ifndef __ENEMY_AI_TACKLE__
#define __ENEMY_AI_TACKLE__

#include "EnemyAIBase.h"

class EnemyAITackle : public EnemyAIBase
{

public:

	static EnemyAITackle *Create();

	// AIÌíÞðh¶æÅZbgµÄ¨­
	virtual const Common::ENEMY_AI GetAIKind() const{ return Common::AI_MOVE_PLAYER; }

protected:
	
	virtual bool InitAI() override;								// AIú»
	virtual void ExecMain( TEX_DRAW_INFO &enemyInfo, ACTION_ARRAY &actionInfo ) override;	// h¶æÅÌAIÀ

private:

	EnemyAITackle(void);
	~EnemyAITackle(void);

};
#endif