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
#include "Game/GameRecorder.h"
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
	const int32_t &GetWalkHeight() const{ return m_walkHeight; }
	const uint32_t &GetEnemyLevel() const;
	const Common::ENEMY_AI GetCurrentAIKind() const;

protected:

	EnemyBase( const std::string &jsonName, const uint32_t &uniqueId, const Common::ENEMY_KIND &kind, const uint32_t &enemyLevel, const math::Vector2 &enemyPos = DEFAULT_VECTOR2 );

	// ��{�p���֎~
	virtual bool Init() override;						// ������
	virtual bool DieMain() override;					// ���S�O����
	virtual void Update() override;						// �ʒu��AI�ɂ�鐔�l����̍X�V
	virtual void DrawUpdate() override;					// �`��X�V

	// �p����ŕK�v�ȏ�������
	virtual bool InitMain(){ return true; }				// �h����ł̏�����
	virtual void UpdateCustom(){}						// �h����ł̍X�V����
	virtual void DrawUpdateCustom(){}					// �h����ł̕`�揈��
	virtual bool DieMainCustom(){ return true; }

	// �ق��̃N���X����̃C�x���g����
	virtual void EventUpdate( Common::CMN_EVENT &eventId ) override;
	virtual void HitPlayreBullet( const uint32_t &damageValue );// �e�������������̏���
	virtual void HitPlayreSlashing( const uint32_t &damageValue );// �a���������������̏���

	// �h����ŃZ�b�g����֐�
	virtual const uint32_t	GetEnemyDefaultHP() const{return 10;}		// �G�N���X�̃f�t�H���gHP�擾
	virtual const float		GetEnemyDefaultSPD() const{return 1.0f;}	// �G�N���X�̃f�t�H���gSPD�擾
	virtual const uint32_t	GetPlayerHitDamage() const{return 10;}		// �v���C���[�Փˎ��̃_���[�W
	virtual const Common::ENEMY_AI GetEnemyDefaultAI() const{return Common::AI_SEARCHING;}	// �G�N���X�̃f�t�H���gSPD�擾
	virtual void  EnemyDeath();											// HP���Ȃ��Ȃ�|���ꂽ�Ƃ��ɌĂ΂��
	virtual void  ReduceDamage( Common::CMN_EVENT &eventId ){};			// �G�̎�ށA���x�����Ń_���[�W�y������

	// �s���s�\���ԃZ�b�g
	void SetStunTime( const uint32_t &stunTime ){ m_stunTime = stunTime; }

	// ���̃N���X�̎�ރZ�b�g
	virtual const Common::TYPE_OBJECT GetTypeObject() const override = 0;

	// �_���[�W���ʏ���
	void UpdateEnemyDamage( const uint32_t &damageValue );

	EnemyAIBase			*m_pEnemyAI;					// �v�l�N���X

private:

	// AI�̍s���𔽉f
	void RefrectAIAction();

	uint32_t			m_uniqueIdOfEnemyAll;			// �G�S�̂̒��ł̎���ID
	Common::ENEMY_KIND	m_enemyKind;					// �G�̎��
	uint32_t			m_enemyLv;						// �G�̋���
	uint32_t			m_HP;							// �G�̗�
	uint32_t			m_speed;						// �G�̃x�[�X�ƂȂ�X�s�[�h
	math::Vector2		m_eye;							// �G�̎���
	int32_t				m_walkHeight;					// �G�̕����鍂��
	uint32_t			m_stunTime;						// ��������̗��R�ɂ���ē����Ȃ�����

	Texture2D			m_textureLife;					// �G���C�t�摜

	ACTION_ARRAY		m_actionInfoAI;					// AI�̎v�l���ʊi�[�ϐ�
	Common::ENEMY_AI	m_nextAI;						// ���Ɏv�l����AI�X�e�[�g
	Common::ENEMY_AI	m_prevAI;						// �ЂƂO��AI�X�e�[�g

};

#endif