/* ====================================================================== */
/**
 * @brief  �GAI��Base�N���X
 *
 * @note
 *		
 */
/* ====================================================================== */

#ifndef __ENEMY_AI_BASE__
#define __ENEMY_AI_BASE__

#include "Game/Enemy/AI/EnemyAIBase.h"
#include "Common/CommonDefine.h"
#include "System/Draw2D/SystemDraw2DDefine.h"

#include "EnemyAIDefine.h"

class EnemyBase;

// ���ʂł̋����̔F��
static const uint32_t DISTANCE_TO_PLAYER_NEAR	= 300;
static const uint32_t DISTANCE_TO_PLAYER_MIDDLE	= 600;
static const uint32_t DISTANCE_TO_PLAYER_FAR	= 1000;

class EnemyAIBase
{

public:

	virtual ~EnemyAIBase(void);

	void SetThingingEnemy( EnemyBase *enemyMine );
	const bool &IsReady(){ return m_isReady; }	// �����������������ǂ���
	void Exec( TEX_DRAW_INFO &enemyInfo, ACTION_ARRAY &actionInfo );		// �v�l�J�n

	// AI�̎�ނ�h����ŃZ�b�g���Ă���
	virtual const Common::ENEMY_AI GetAIKind() const = 0 ;

	// �G�L�������̂��̂Ɋւ���get,set�֐�
	void SetEnemyAnim( const std::string &animTag );
	std::string GetEnemyAnim();
	void SetEnemyEyeSight( math::Vector2 &eye );
	math::Vector2 GetEnemyEyeSight() const;
	void ChangeEnemyAI( Common::ENEMY_AI nextAI );
	Common::ENEMY_KIND GetEnemyKind() const;
	const std::string &GetEnemyJsonName() const;

protected:
	
	EnemyAIBase();

	virtual bool InitAI(){ return true; }					// AI������
	virtual void ExecMain( TEX_DRAW_INFO &enemyInfo, ACTION_ARRAY &actionInfo ) = 0;	// �h����ł�AI����
		
private:
	
	EnemyAIBase( EnemyBase *enemyMine );
	bool		m_isReady;		// �v�l�������I���������ǂ���
	EnemyBase	*m_enemyMine;	// ���݂̎����̏�Ԃ�m�邽�߂ɕێ����Ă���
};
#endif