/* ====================================================================== */
/**
 * @brief  �GAI�̒ʏ�N���X(�v���C���[������)
 *
 * @note
 *		�f�t�H���gAI
 */
/* ====================================================================== */

#ifndef __AI_BOSS_MOVING__
#define __AI_BOSS_MOVING__

#include "Game/Enemy/AI/EnemyAIBase.h"

class AIBossMoving : public EnemyAIBase
{
public:

	static AIBossMoving *Create();

	// AI�̎�ނ�h����ŃZ�b�g���Ă���
	virtual const Common::ENEMY_AI GetAIKind() const{ return Common::AI_SEARCHING; }

protected:
	
	virtual bool InitAI() override;								// AI������
	virtual void ExecMain( TEX_DRAW_INFO &enemyInfo, ACTION_ARRAY &actionInfo ) override;	// �h����ł�AI����

private:

	bool SearchPlayer( TEX_DRAW_INFO &enemyInfo );				// �v���C���[�T��

	AIBossMoving(void);
	~AIBossMoving(void);

	math::Vector2	m_targetPlace;
};

#endif