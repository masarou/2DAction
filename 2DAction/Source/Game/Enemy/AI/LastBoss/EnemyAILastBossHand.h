/* ====================================================================== */
/**
 * @brief  ���X�{�X����AI�N���X
 *
 * @note
 *		�f�t�H���gAI
 */
/* ====================================================================== */

#ifndef __ENEMY_AI_LAST_BOSS_HAND__
#define __ENEMY_AI_LAST_BOSS_HAND__

#include "Game/Enemy/AI/EnemyAIBase.h"

class EnemyAILastBossHand : public EnemyAIBase
{
public:

	static EnemyAILastBossHand *Create();

	// AI�̎�ނ�h����ŃZ�b�g���Ă���
	virtual const Common::ENEMY_AI GetAIKind() const{ return Common::AI_ATTACK_LAST_BOSS_HAND; }

protected:
	
	virtual bool InitAI() override;								// AI������
	virtual void ExecMain( TEX_DRAW_INFO &enemyInfo, ACTION_ARRAY &actionInfo ) override;	// �h����ł�AI����

private:

	EnemyAILastBossHand(void);
	~EnemyAILastBossHand(void);

};

#endif