/* ====================================================================== */
/**
 * @brief  �X���C���L���OAI�̒ʏ�N���X(�v���C���[������)
 *
 * @note
 *		�f�t�H���gAI
 */
/* ====================================================================== */

#ifndef __ENEMY_AI_KING_SLIME_SEARCHING__
#define __ENEMY_AI_KING_SLIME_SEARCHING__

#include "Game/Enemy/AI/EnemyAIBase.h"

class AISlimeKingSearching : public EnemyAIBase
{
public:

	static AISlimeKingSearching *Create();

	// AI�̎�ނ�h����ŃZ�b�g���Ă���
	virtual const Common::ENEMY_AI GetAIKind() const{ return Common::AI_SEARCHING_SLIME_KING; }

protected:
	
	virtual bool InitAI() override;								// AI������
	virtual void ExecMain( TEX_DRAW_INFO &enemyInfo, ACTION_ARRAY &actionInfo ) override;	// �h����ł�AI����

private:

	bool SearchPlayer( TEX_DRAW_INFO &enemyInfo, float distance = 300.0f );				// �v���C���[�T��

	AISlimeKingSearching(void);
	~AISlimeKingSearching(void);

	uint32_t	m_circleRadius;		// �p�j�p�̉~�̑傫��(���a)
	uint32_t	m_circleDistance;	// ���g�̒n�_����p�j�~�܂ł̋���

};

#endif