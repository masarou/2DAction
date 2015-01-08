/* ====================================================================== */
/**
 * @brief  GAIÌUNX(£)
 *
 * @note
 *		UAI
 */
/* ====================================================================== */

#include "EnemyAIShoot.h"

EnemyAIShoot *EnemyAIShoot::Create()
{
	EnemyAIShoot *tmpAI = NEW EnemyAIShoot();
	return tmpAI;
}

EnemyAIShoot::EnemyAIShoot(void)
{
}


EnemyAIShoot::~EnemyAIShoot(void)
{
}

void EnemyAIShoot::ExecMain( TEX_DRAW_INFO &enemyInfo, ACTION_INFO &actionInfo )
{
	if( GetRandamValue( 120, 0 ) == 0 ){
		// vC[Éü©ÁÄeð­Ë
		actionInfo.m_pushEventArray.push_back( Common::EVENT_SHOOT_BULLET );
	}
}
