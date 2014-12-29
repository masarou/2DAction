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

#include "EnemyBase.h"
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

	// �G�L�������̂��̂Ɋւ���get,set�֐�
	void SetEnemyAnim( const std::string &animTag );
	std::string GetEnemyAnim();
	void SetEnemyEyeSight( math::Vector2 &eye );
	const math::Vector2 &GetEnemyEyeSight() const;
	void ChangeEnemyAI( Common::ENEMY_AI nextAI );

protected:
	
	EnemyAIBase();

	virtual bool InitAI(){ return true; }					// AI������
	virtual void ExecMain( TEX_DRAW_INFO &enemyInfo ){};	// �h����ł�AI����
		
private:
	
	EnemyAIBase( EnemyBase *enemyMine );
	bool		m_isReady;		// �v�l�������I���������ǂ���
	EnemyBase	*m_enemyMine;	// ���݂̎����̏�Ԃ�m�邽�߂ɕێ����Ă���
};
#endif