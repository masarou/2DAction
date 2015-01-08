/* ====================================================================== */
/**
 * @brief  �G�Ǘ��N���X
 *
 * @note
 *		
 */
/* ====================================================================== */

#ifndef ENEMY_BASE
#define ENEMY_BASE

#include "Common/CommonDefine.h"
#include "Common/Utility/CommonGameUtility.h"
#include "EnemyAIDefine.h"
#include "EnemyAIBase.h"
#include "EnemyManager.h"
#include "Game/Game2DBase.h"
#include "System/SystemDefine.h"
#include "System/Message/SystemMessageUnit.h"

class AttackGun;

class EnemyBase : public SystemMessageUnit
{
	
	friend EnemyAIBase;
	friend EnemyManager;

public:

	enum ENEMY_STATE{
		ENEMY_INIT,
		ENEMY_ACTION,

		ENEMY_MAX
	};

	virtual ~EnemyBase(void);

	// �ق��̃N���X����̃C�x���g����
	void EventUpdate( const Common::CMN_EVENT &eventId ) override;

	// ���擾�֐�
	const ENEMY_STATE &GetState() const{ return m_enemyState; }
	const Common::ENEMY_KIND &GetKind() const{ return m_enemyKind; }
	const TEX_DRAW_INFO &GetDrawInfo() const;
	const uint32_t &GetEnemyHitPoint() const{ return m_HP; }
	const uint32_t &GetUniqueNumber() const{ return m_uniqueIdOfEnemyAll; }
	const math::Vector2 &GetEnemyEyeSight() const{ return m_eye; }

protected:

	EnemyBase( const std::string &jsonName, const uint32_t &uniqueId, const Common::ENEMY_KIND &kind );

	bool Init();										// ������
	virtual bool InitMain(){ return true; }				// �h����ł̏�����
	virtual void UpdateEnemy();							// �ʒu��AI�ɂ�鐔�l����̍X�V
	virtual void DrawEnemy();							// �`��X�V
	virtual void HitPlayreBullet( uint32_t damageValue );// �e�������������̏���

	// �h����ŃZ�b�g����֐�
	virtual uint32_t GetEnemyDefaultHP() const{return 10;}	// �G�N���X�̃f�t�H���gHP�擾

	EnemyAIBase			*m_pEnemyAI;					// �v�l�N���X
	Texture2D			m_textureEnemy;					// �G�摜

private:

	// AI�̍s���𔽉f
	void RefrectAIAction();

	ENEMY_STATE			m_enemyState;					// ���̃N���X�̏��
	uint32_t			m_uniqueIdOfEnemyAll;			// �G�S�̂̒��ł̎���ID
	Common::ENEMY_KIND	m_enemyKind;					// �G�̎��
	uint32_t			m_HP;							// �G�̗�
	math::Vector2		m_eye;							// �G�̎���
	TEX_DRAW_INFO		m_enemyInfo;					// �`��֌W���

	Texture2D			m_textureLife;					// �G���C�t�摜

	ACTION_INFO			m_actionInfoAI;					// AI�̎v�l���ʊi�[�ϐ�
	Common::ENEMY_AI	m_nextAI;						// ���Ɏv�l����AI�X�e�[�g
	Common::ENEMY_AI	m_prevAI;						// �ЂƂO��AI�X�e�[�g

protected:

	// ���ׂĂ̓G�N���X�ŋ��L
	static AttackGun	*s_pAttackGun;		// �}�V���K���N���X

};

#endif