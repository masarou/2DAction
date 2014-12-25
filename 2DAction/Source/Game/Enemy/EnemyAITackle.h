/* ====================================================================== */
/**
 * @brief  GAIÌUNX(ß£)
 *
 * @note
 *		UAI
 */
/* ====================================================================== */

#ifndef ENEMY_AI_TACKLE
#define ENEMY_AI_TACKLE

#include "EnemyAIBase.h"

class EnemyAITackle : public EnemyAIBase
{

public:

	static EnemyAITackle *Create();

	// AIÌíÞðh¶æÅZbgµÄ¨­
	virtual const Common::ENEMY_AI GetAIKind() const{ return Common::AI_MOVE_PLAYER; }

protected:
	
	virtual bool InitAI() override{ return true; }					// AIú»
	virtual void ExecMain( TEX_DRAW_INFO &enemyInfo ) override{};	// h¶æÅÌAIÀ

private:

	EnemyAITackle(void);
	~EnemyAITackle(void);

};
#endif