/* ====================================================================== */
/**
 * @brief  GAIÌUNX(£)
 *
 * @note
 *		UAI
 */
/* ====================================================================== */

#ifndef __ENEMY_AI_SHOOT__
#define __ENEMY_AI_SHOOT__

#include "EnemyAIBase.h"

class EnemyAIShoot : public EnemyAIBase
{
public:

	static EnemyAIShoot *Create();

	// AIÌíÞðh¶æÅZbgµÄ¨­
	virtual const Common::ENEMY_AI GetAIKind() const{ return Common::AI_SHOOTING; }

protected:
	
	virtual bool InitAI() override{ return true; }					// AIú»
	virtual void ExecMain( TEX_DRAW_INFO &enemyInfo, ACTION_INFO &actionInfo ) override;		// h¶æÅÌAIÀ

private:

	EnemyAIShoot(void);
	~EnemyAIShoot(void);
};
#endif
