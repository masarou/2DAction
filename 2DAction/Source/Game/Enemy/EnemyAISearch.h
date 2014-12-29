/* ====================================================================== */
/**
 * @brief  �GAI�̒ʏ�N���X(�v���C���[������)
 *
 * @note
 *		�f�t�H���gAI
 */
/* ====================================================================== */

#ifndef ENEMY_AI_SEARCH
#define ENEMY_AI_SEARCH

#include "EnemyAIBase.h"

class EnemyAISearch : public EnemyAIBase
{
public:

	static EnemyAISearch *Create();

	// AI�̎�ނ�h����ŃZ�b�g���Ă���
	virtual const Common::ENEMY_AI GetAIKind() const{ return Common::AI_SEARCHING; }

protected:
	
	virtual bool InitAI() override;								// AI������
	virtual void ExecMain( TEX_DRAW_INFO &enemyInfo ) override;	// �h����ł�AI����

private:

	bool SearchPlayer( TEX_DRAW_INFO &enemyInfo );				// �v���C���[�T��

	EnemyAISearch(void);
	~EnemyAISearch(void);

	uint32_t	m_circleRadius;		// �p�j�p�̉~�̑傫��(���a)
	uint32_t	m_circleDistance;	// ���g�̒n�_����p�j�~�܂ł̋���

};

#endif