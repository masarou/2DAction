/* ====================================================================== */
/**
 * @brief  敵AIの攻撃クラス(遠距離)
 *
 * @note
 *		攻撃AI
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
	if( Utility::GetRandamValue( 120, 0 ) == 0 ){
		// プレイヤーに向かって弾を発射
		actionInfo.m_pushEventArray.push_back( Common::EVENT_SHOOT_BULLET );
	}
}
