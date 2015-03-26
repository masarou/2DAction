/* ====================================================================== */
/**
 * @brief  “GAI‚ÌUŒ‚ƒNƒ‰ƒX(‹ß‹——£)
 *
 * @note
 *		UŒ‚AI
 */
/* ====================================================================== */

#ifndef __ENEMY_AI_TACKLE__
#define __ENEMY_AI_TACKLE__

#include "EnemyAIBase.h"

class EnemyAITackle : public EnemyAIBase
{

public:

	static EnemyAITackle *Create();

	// AI‚Ìí—Ş‚ğ”h¶æ‚ÅƒZƒbƒg‚µ‚Ä‚¨‚­
	virtual const Common::ENEMY_AI GetAIKind() const{ return Common::AI_MOVE_PLAYER; }

protected:
	
	virtual bool InitAI() override;								// AI‰Šú‰»
	virtual void ExecMain( TEX_DRAW_INFO &enemyInfo, ACTION_ARRAY &actionInfo ) override;	// ”h¶æ‚Å‚ÌAIÀ‘•

private:

	EnemyAITackle(void);
	~EnemyAITackle(void);

};
#endif