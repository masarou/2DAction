/* ====================================================================== */
/**
 * @brief  �GAI�̒ʏ�N���X(�v���C���[������)
 *
 * @note
 *		�f�t�H���gAI
 */
/* ====================================================================== */

#ifndef __AI_BOSS_NEAE_ATTACK__
#define __AI_BOSS_NEAE_ATTACK__

#include "Game/Enemy/AI/EnemyAIBase.h"

class AIBossNearAttack : public EnemyAIBase
{
public:

	static AIBossNearAttack *Create();

	// AI�̎�ނ�h����ŃZ�b�g���Ă���
	virtual const Common::ENEMY_AI GetAIKind() const{ return Common::AI_SEARCHING; }

protected:
	
	virtual bool InitAI() override;								// AI������
	virtual void ExecMain( TEX_DRAW_INFO &enemyInfo, ACTION_ARRAY &actionInfo ) override;	// �h����ł�AI����

private:

	enum ACTION_TYPE{
		ACTION_SPREAD_BULLET,
		ACTION_SLASHING,
		ACTION_RUN_AWAY,

		ACTION_MAX,
	};

	bool ExecSpreadBullet( TEX_DRAW_INFO &enemyInfo, ACTION_ARRAY &actionInfo );	// �V���b�g�K���I�ȍU��
	bool ExecSlashing( TEX_DRAW_INFO &enemyInfo, ACTION_ARRAY &actionInfo );		// �a���U��
	bool ExecRunAway( TEX_DRAW_INFO &enemyInfo, ACTION_ARRAY &actionInfo );		// �������Ƃ�

	// �U���֐�
	void CreateSpreadBullet( ACTION_ARRAY &actionInfo );

	AIBossNearAttack(void);
	~AIBossNearAttack(void);

	math::Vector2	m_targetPlace;
	ACTION_TYPE		m_actionType;
};

#endif