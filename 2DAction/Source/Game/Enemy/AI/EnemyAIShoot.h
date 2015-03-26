/* ====================================================================== */
/**
 * @brief  �GAI�̍U���N���X(������)
 *
 * @note
 *		�U��AI
 */
/* ====================================================================== */

#ifndef __ENEMY_AI_SHOOT__
#define __ENEMY_AI_SHOOT__

#include "EnemyAIBase.h"

class EnemyAIShoot : public EnemyAIBase
{
public:

	static EnemyAIShoot *Create();

	// AI�̎�ނ�h����ŃZ�b�g���Ă���
	virtual const Common::ENEMY_AI GetAIKind() const{ return Common::AI_SHOOTING; }

protected:
	
	virtual bool InitAI() override{ return true; }					// AI������
	virtual void ExecMain( TEX_DRAW_INFO &enemyInfo, ACTION_ARRAY &actionInfo ) override;		// �h����ł�AI����

private:

	EnemyAIShoot(void);
	~EnemyAIShoot(void);
};
#endif
