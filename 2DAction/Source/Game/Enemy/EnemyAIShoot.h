/* ====================================================================== */
/**
 * @brief  GAIÌUNX(£)
 *
 * @note
 *		UAI
 */
/* ====================================================================== */

#ifndef ENEMY_AI_SHOOT
#define ENEMY_AI_SHOOT

#include "EnemyAIBase.h"

class EnemyAIShoot : public EnemyAIBase
{
public:

	static EnemyAIShoot *Create();

	// AIÌíÞðh¶æÅZbgµÄ¨­
	virtual const Common::ENEMY_AI GetAIKind() const{ return Common::AI_SHOOTING; }

protected:
	
	virtual bool InitAI() override{ return true; }					// AIú»
	virtual void ExecMain( TEX_DRAW_INFO &enemyInfo ) override{};	// h¶æÅÌAIÀ

private:

	EnemyAIShoot(void);
	~EnemyAIShoot(void);
};
#endif
