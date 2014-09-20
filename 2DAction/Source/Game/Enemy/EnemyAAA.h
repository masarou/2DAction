/* ====================================================================== */
/**
 * @brief  ìGä«óùÉNÉâÉX
 *
 * @note
 *		
 */
/* ====================================================================== */

#ifndef ENEMY_AAA
#define ENEMY_AAA

#include "EnemyBase.h"

class EnemyAAA : public EnemyBase
{
public:

	static EnemyAAA *Create( uint32_t uniqueID );

protected:
	
	virtual void UpdateEnemy() override;

private:

	EnemyAAA( uint32_t uniqueID );
	~EnemyAAA(void);

};

#endif ENEMY_AAA