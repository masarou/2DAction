/* ====================================================================== */
/**
 * @brief  �GAI�̍U���N���X(�ߋ���)
 *
 * @note
 *		�U��AI
 */
/* ====================================================================== */

#ifndef __ENEMY_AI_TACKLE__
#define __ENEMY_AI_TACKLE__

#include "EnemyAIBase.h"

class EnemyAITackle : public EnemyAIBase
{

public:

	static EnemyAITackle *Create();

	// AI�̎�ނ�h����ŃZ�b�g���Ă���
	virtual const Common::ENEMY_AI GetAIKind() const{ return Common::AI_MOVE_PLAYER; }

protected:
	
	virtual bool InitAI() override;								// AI������
	virtual void ExecMain( TEX_DRAW_INFO &enemyInfo, ACTION_ARRAY &actionInfo ) override;	// �h����ł�AI����

private:

	EnemyAITackle(void);
	~EnemyAITackle(void);

};
#endif