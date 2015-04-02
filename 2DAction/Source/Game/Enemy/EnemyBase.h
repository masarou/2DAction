/* ====================================================================== */
/**
 * @brief  �G�Ǘ��N���X
 *
 * @note
 *		
 */
/* ====================================================================== */

#ifndef __ENEMY_BASE__
#define __ENEMY_BASE__

#include "Common/CommonDefine.h"
#include "Common/Utility/CommonGameUtility.h"
#include "AI/EnemyAIDefine.h"
#include "AI/EnemyAIBase.h"
#include "EnemyManager.h"
#include "Game/Effect/GameEffect.h"
#include "System/Task/SystemTaskUnit.h"
#include "System/SystemDefine.h"
#include "System/Collision/SystemCollisionUnit.h"

class EnemyBase : public TaskUnit, public Collision2DUnit
{
	
	friend EnemyAIBase;
	friend EnemyManager;

public:

	virtual ~EnemyBase(void);

	// ���擾�֐�
	const Common::ENEMY_KIND &GetKind() const{ return m_enemyKind; }
	const TEX_DRAW_INFO &GetDrawInfo() const;
	const uint32_t &GetEnemyHitPoint() const{ return m_HP; }
	const uint32_t &GetUniqueNumber() const{ return m_uniqueIdOfEnemyAll; }
	const math::Vector2 &GetEnemyEyeSight() const{ return m_eye; }
	const uint32_t &GetWalkHeight() const{ return m_walkHeight; }

protected:

	EnemyBase( const std::string &jsonName, const uint32_t &uniqueId, const Common::ENEMY_KIND &kind );

	virtual bool Init() override;						// ������
	virtual bool InitMain(){ return true; }				// �h����ł̏�����
	virtual bool DieMain() override;					// �h����ł̏�����
	virtual void Update() override;						// �ʒu��AI�ɂ�鐔�l����̍X�V
	virtual void DrawUpdate() override;					// �`��X�V

	// �ق��̃N���X����̃C�x���g����
	virtual void EventUpdate( const Common::CMN_EVENT &eventId ) override;
	virtual void HitPlayreBullet( const uint32_t &damageValue );// �e�������������̏���
	virtual void HitPlayreSlashing( const uint32_t &damageValue );// �a���������������̏���

	// �h����ŃZ�b�g����֐�
	virtual uint32_t GetEnemyDefaultHP() const{return 10;}	// �G�N���X�̃f�t�H���gHP�擾

	// ���̃N���X�̎�ރZ�b�g
	virtual const Common::TYPE_OBJECT GetTypeObject() const override{ return Common::TYPE_EVENMY_AAA; }

	// ���̃N���X�Œ�`���鉼�z�֐�
	virtual bool DieMainCustom(){ return true; }

	EnemyAIBase			*m_pEnemyAI;					// �v�l�N���X

private:

	// �_���[�W���ʏ���
	void UpdateEnemyDamage( const uint32_t &damageValue );

	// AI�̍s���𔽉f
	void RefrectAIAction();

	uint32_t			m_uniqueIdOfEnemyAll;			// �G�S�̂̒��ł̎���ID
	Common::ENEMY_KIND	m_enemyKind;					// �G�̎��
	uint32_t			m_HP;							// �G�̗�
	math::Vector2		m_eye;							// �G�̎���
	uint32_t			m_walkHeight;					// �G�̕����鍂��
	uint32_t			m_stunTime;						// ��������̗��R�ɂ���ē����Ȃ�����

	Texture2D			m_textureLife;					// �G���C�t�摜

	ACTION_ARRAY		m_actionInfoAI;					// AI�̎v�l���ʊi�[�ϐ�
	Common::ENEMY_AI	m_nextAI;						// ���Ɏv�l����AI�X�e�[�g
	Common::ENEMY_AI	m_prevAI;						// �ЂƂO��AI�X�e�[�g

};

#endif