/* ====================================================================== */
/**
 * @brief  “GAI‚ÌUŒ‚ƒNƒ‰ƒX(‰“‹——£)
 *
 * @note
 *		UŒ‚AI
 */
/* ====================================================================== */

#ifndef __ENEMY_AI_SHOOT__
#define __ENEMY_AI_SHOOT__

#include "EnemyAIBase.h"

class EnemyAIShoot : public EnemyAIBase
{
public:

	// ’e‚ğ”­Ë‚µ‚Ä‚©‚ç‚ÌÅ’á‘Ò‚¿ŠÔ
	static const uint32_t ACTION_INTERVAL = 60;

	static EnemyAIShoot *Create();

	// AI‚Ìí—Ş‚ğ”h¶æ‚ÅƒZƒbƒg‚µ‚Ä‚¨‚­
	virtual const Common::ENEMY_AI GetAIKind() const{ return Common::AI_SHOOTING; }

protected:
	
	virtual bool InitAI() override{ return true; }					// AI‰Šú‰»
	virtual void ExecMain( TEX_DRAW_INFO &enemyInfo, ACTION_ARRAY &actionInfo ) override;		// ”h¶æ‚Å‚ÌAIÀ‘•

private:

	EnemyAIShoot(void);
	~EnemyAIShoot(void);

	uint32_t	m_shootInterval;	// ˜A‘±‚µ‚Ä”­Ë‚·‚é‚Ü‚Å‚ÌÅ’áŠÔ
};
#endif
