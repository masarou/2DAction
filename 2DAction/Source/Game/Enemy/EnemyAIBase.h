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

#include "Common/CommonDefine.h"
#include "System/Draw2D/SystemDraw2DDefine.h"

class EnemyBase;

class EnemyAIBase
{

public:

	virtual ~EnemyAIBase(void);

	void SetThingingEnemy( EnemyBase *enemyMine );
	const bool &IsReady(){ return m_isReady; }	// �����������������ǂ���
	void Exec( TEX_DRAW_INFO &enemyInfo );		// �v�l�J�n

	// AI�̎�ނ�h����ŃZ�b�g���Ă���
	virtual const Common::ENEMY_AI GetAIKind() const = 0 ;

protected:
	
	EnemyAIBase();

	virtual bool InitAI(){ return true; }					// AI������
	virtual void ExecMain( TEX_DRAW_INFO &enemyInfo ){};	// �h����ł�AI����

private:
	
	EnemyAIBase( EnemyBase *enemyMine );
	
	// ���݂̎����̏�Ԃ�m�邽�߂ɕێ����Ă���
	EnemyBase	*m_enemyMine;
	bool		m_isReady;		// �v�l�������I���������ǂ���
};
#endif