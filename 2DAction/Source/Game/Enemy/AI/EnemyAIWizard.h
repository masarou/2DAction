/* ====================================================================== */
/**
 * @brief  �GWizard�N���XAI
 *
 * @note
 *		�f�t�H���gAI
 */
/* ====================================================================== */

#ifndef __ENEMY_AI_WIZARD__
#define __ENEMY_AI_WIZARD__

#include "EnemyAIBase.h"

class AIWizard : public EnemyAIBase
{
public:

	static AIWizard *Create();

	// AI�̎�ނ�h����ŃZ�b�g���Ă���
	virtual const Common::ENEMY_AI GetAIKind() const{ return Common::AI_ATTACK_WIZARD; }

protected:
	
	virtual bool InitAI() override;								// AI������
	virtual void ExecMain( TEX_DRAW_INFO &enemyInfo, ACTION_ARRAY &actionInfo ) override;	// �h����ł�AI����

private:

	bool SearchPlayer( TEX_DRAW_INFO &enemyInfo, float distance = 300.0f );				// �v���C���[�T��

	AIWizard(void);
	~AIWizard(void);

	uint32_t	m_circleRadius;		// �p�j�p�̉~�̑傫��(���a)
	uint32_t	m_circleDistance;	// ���g�̒n�_����p�j�~�܂ł̋���

};


#endif