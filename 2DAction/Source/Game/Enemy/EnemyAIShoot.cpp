/* ====================================================================== */
/**
 * @brief  �GAI�̍U���N���X(������)
 *
 * @note
 *		�U��AI
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
		// �v���C���[�Ɍ������Ēe�𔭎�
		actionInfo.m_pushEventArray.push_back( Common::EVENT_SHOOT_BULLET );
	}
}
