/* ====================================================================== */
/**
 * @brief  �X���C���L���O�̍U���N���X(�ߋ���)
 *
 * @note
 *		�U��AI
 */
/* ====================================================================== */

#ifndef __ENEMY_AI_SLIME_KING_TACKLE__
#define __ENEMY_AI_SLIME_KING_TACKLE__

#include "Game/Enemy/AI/EnemyAIBase.h"

class AISlimeKingTackle : public EnemyAIBase
{

public:

	static AISlimeKingTackle *Create();

	// AI�̎�ނ�h����ŃZ�b�g���Ă���
	virtual const Common::ENEMY_AI GetAIKind() const{ return Common::AI_MOVE_PLAYER_SLIME_KING; }

protected:
	
	virtual bool InitAI() override;								// AI������
	virtual void ExecMain( TEX_DRAW_INFO &enemyInfo, ACTION_ARRAY &actionInfo ) override;	// �h����ł�AI����

private:

	AISlimeKingTackle(void);
	~AISlimeKingTackle(void);

};
#endif