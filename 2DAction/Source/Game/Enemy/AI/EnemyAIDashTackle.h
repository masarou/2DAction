/* ====================================================================== */
/**
 * @brief  �GAI�̍U���N���X(�_�b�V���œˌ�)
 *
 * @note
 *		�U��AI
 */
/* ====================================================================== */

#ifndef __ENEMY_AI_DASH_TACKLE__
#define __ENEMY_AI_DASH_TACKLE__

#include "EnemyAIBase.h"

class EnemyAIDashTackle : public EnemyAIBase
{

public:

	static EnemyAIDashTackle *Create();

	// AI�̎�ނ�h����ŃZ�b�g���Ă���
	virtual const Common::ENEMY_AI GetAIKind() const{ return Common::AI_DASH_TACKLE; }

protected:
	
	virtual bool InitAI() override;								// AI������
	virtual void ExecMain( TEX_DRAW_INFO &enemyInfo, ACTION_ARRAY &actionInfo ) override;	// �h����ł�AI����

	void UpdateIdle( TEX_DRAW_INFO &enemyInfo, ACTION_ARRAY &actionInfo );
	void UpdateAction( TEX_DRAW_INFO &enemyInfo, ACTION_ARRAY &actionInfo );
	void UpdatePreEnd( TEX_DRAW_INFO &enemyInfo, ACTION_ARRAY &actionInfo );

private:

	enum TACKLE_STATE{
		IDLE,
		ACTION,
		PRE_END,

		MAX,
	};

	EnemyAIDashTackle(void);
	~EnemyAIDashTackle(void);

	TACKLE_STATE	m_tackleState;
	uint32_t		m_actionTime;		// ACTION�ɃX�e�[�^�X���ڂ��Ă���̎���
	math::Vector2	m_tackleMoveVec;	// �^�b�N������
	float			m_actionAccelSpeed;	// �^�b�N���X�s�[�h
	float			m_tacklePower;		// �^�b�N����

};
#endif