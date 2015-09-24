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
#include "Game/Attack/GameAttackBlade.h"

#include "EnemyAIDefine.h"

class EnemyBase;
class AttackGun;

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
	const uint32_t GetEnemyLevel() const;
	const uint32_t GetEnemySPD() const;

	const EnemyBase *const GetEnemeyMine() const;

	// �U���N���X���
	static void ClearAttackMaterial();

	// ����AI�ɂȂ��Ă���̌o�ߎ��Ԃ��擾( 1000 = 1s )
	uint32_t GetSecStartThisAI();

protected:
	
	EnemyAIBase();

	virtual bool InitAI(){ return true; }					// AI������
	virtual void ExecMain( TEX_DRAW_INFO &enemyInfo, ACTION_ARRAY &actionInfo ) = 0;	// �h����ł�AI����
	
	// �ȉ��A�K�v�ɂȂ�ł��낤�֗��֐�
	void ShootBullet( const math::Vector2 &pos = math::Vector2(), const math::Vector2 &vec = math::Vector2(), const uint32_t &damage = 20.0f, const uint32_t &speed = 10.0f );	// �U���e����
	void Slashing( const Slashing::TYPE_SLASHING &type, const math::Vector2 slashDir = math::Vector2(), const math::Vector2 &vec = math::Vector2() );

	// ���ׂĂ̓G�N���X�ŋ��L
	static AttackGun	*s_pAttackGun;		// �}�V���K���N���X
	static AttackBlade	*s_pAttackBlade;	// �a���N���X

private:
	
	uint32_t	m_thisAIStartTime;	// ����AI�ɐ؂�ւ���Ă���̎���
	bool		m_isReady;			// �v�l�������I���������ǂ���
	EnemyBase	*m_enemyMine;		// ���݂̎����̏�Ԃ�m�邽�߂ɕێ����Ă���
};
#endif