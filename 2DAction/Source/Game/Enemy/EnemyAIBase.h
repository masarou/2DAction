/* ====================================================================== */
/**
 * @brief  �GAI��Base�N���X
 *
 * @note
 *		
 */
/* ====================================================================== */

#ifndef ENEMY_AI_BASE
#define ENEMY_AI_BASE

#include "System/Draw2D/SystemDraw2DDefine.h"

class EnemyBase;

class EnemyAIBase
{

public:

	//static EnemyAIBase *Create( EnemyBase *enemyMine = NULL );
	virtual ~EnemyAIBase(void);

	void SetThingingEnemy( EnemyBase *enemyMine );
	const bool &IsReady(){ return m_isReady; }	// �����������������ǂ���
	void Exec( TEX_DRAW_INFO &enemyInfo );		// �v�l�J�n

protected:

	virtual bool InitAI(){ return true; }	// AI������
	virtual void ExecMain( TEX_DRAW_INFO &enemyInfo ){};	// �h����ł�AI����

private:
	
	EnemyAIBase();
	EnemyAIBase( EnemyBase *enemyMine );
	
	// ���݂̎����̏�Ԃ�m�邽�߂ɕێ����Ă���
	EnemyBase	*m_enemyMine;
	bool		m_isReady;		// �v�l�������I���������ǂ���
};
#endif