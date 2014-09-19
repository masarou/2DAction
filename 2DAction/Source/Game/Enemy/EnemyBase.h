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

class EnemyBase : public TaskUnit
{
private:


protected:

	EnemyBase( std::string jsonName, uint32_t uniqueId, Common::ENEMY_KIND kind );
	virtual ~EnemyBase(void);

	virtual void DrawUpdate() override;
	virtual bool DieMain() override;

	// ‘¼ƒNƒ‰ƒX‚©‚ç‚ÌƒCƒxƒ“ƒgˆ—
	void EventUpdate( const Common::CMN_EVENT &eventId ) override;
	void HitPlayreBullet();

public:

	uint32_t			m_uniqueIdOfEnemyAll;	// “G‘S‘Ì‚Ì’†‚Å‚Ì¯•ÊID
	Common::ENEMY_KIND	m_enemyKind;			// “G‚Ìí—Ş

	uint32_t			m_HP;					// “G‘Ì—Í
	math::Vector2		m_eye;					// “G‚Ì‹ü
	Game2DBase			*m_enemy2D;				// “G‚Ì‰æ‘œ“™ŠÇ—
	TEX_DRAW_INFO		m_enemyInfo;			// •`‰æŠÖŒWî•ñ
};

#endif