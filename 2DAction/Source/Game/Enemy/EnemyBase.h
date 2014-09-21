/* ====================================================================== */
/**
 * @brief  “GŠÇ—ƒNƒ‰ƒX
 *
 * @note
 *		
 */
/* ====================================================================== */

#ifndef ENEMY_BASE
#define ENEMY_BASE

#include "System/SystemDefine.h"
#include "Game/Game2DBase.h"

class EnemyBase
{

public:

	virtual ~EnemyBase(void);

	virtual void UpdateEnemy(){};							// ˆÊ’u‚âAI‚É‚æ‚é”’lü‚è‚ÌXV
	virtual void DrawEnemy();								// •`‰æXV
	void EventUpdate( const Common::CMN_EVENT &eventId );	// manager‚©‚ç‚ÌƒCƒxƒ“ƒgˆ—

	const TEX_DRAW_INFO &GetDrawInfo();
	uint32_t GetUniqueNumber(){ return m_uniqueIdOfEnemyAll; }

protected:

	EnemyBase( std::string jsonName, uint32_t uniqueId, Common::ENEMY_KIND kind );


	void HitPlayreBullet();			// ’e‚ª“–‚½‚Á‚½‚Ìˆ—

protected:

	uint32_t			m_uniqueIdOfEnemyAll;	// “G‘S‘Ì‚Ì’†‚Å‚Ì¯•ÊID
	Common::ENEMY_KIND	m_enemyKind;			// “G‚Ìí—Ş

	uint32_t			m_HP;					// “G‘Ì—Í
	math::Vector2		m_eye;					// “G‚Ì‹ü
	Game2DBase			*m_enemy2D;				// “G‚Ì‰æ‘œ“™ŠÇ—
	TEX_DRAW_INFO		m_enemyInfo;			// •`‰æŠÖŒWî•ñ
};

#endif