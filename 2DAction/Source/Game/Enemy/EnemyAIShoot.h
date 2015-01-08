/* ====================================================================== */
/**
 * @brief  “GAI‚ÌUŒ‚ƒNƒ‰ƒX(‰“‹——£)
 *
 * @note
 *		UŒ‚AI
 */
/* ====================================================================== */

#ifndef ENEMY_AI_SHOOT
#define ENEMY_AI_SHOOT

#include "EnemyAIBase.h"

class EnemyAIShoot : public EnemyAIBase
{
public:

	static EnemyAIShoot *Create();

	// AI‚Ìí—Ş‚ğ”h¶æ‚ÅƒZƒbƒg‚µ‚Ä‚¨‚­
	virtual const Common::ENEMY_AI GetAIKind() const{ return Common::AI_SHOOTING; }

protected:
	
	virtual bool InitAI() override{ return true; }					// AI‰Šú‰»
	virtual void ExecMain( TEX_DRAW_INFO &enemyInfo, ACTION_INFO &actionInfo ) override;		// ”h¶æ‚Å‚ÌAIÀ‘•

private:

	EnemyAIShoot(void);
	~EnemyAIShoot(void);
};
#endif
