/* ====================================================================== */
/**
 * @brief  ���X�{�XAI
 *
 * @note
 *		
 */
/* ====================================================================== */

#ifndef __ENEMY_AI_LAST_BOSS__
#define __ENEMY_AI_LAST_BOSS__

#include "Game/Enemy/AI/EnemyAIBase.h"

class LastBossRight;
class LastBossLeft;

class EnemyAILastBoss : public EnemyAIBase
{
public:

	static EnemyAILastBoss *Create();

	// AI�̎�ނ�h����ŃZ�b�g���Ă���
	virtual const Common::ENEMY_AI GetAIKind() const{ return Common::AI_ATTACK_LAST_BOSS; }

protected:
	
	virtual bool InitAI() override;								// AI������
	virtual void ExecMain( TEX_DRAW_INFO &enemyInfo, ACTION_ARRAY &actionInfo ) override;	// �h����ł�AI����
	virtual void EnemyIsDead();

private:

	void ExecHandsUpdate( TEX_DRAW_INFO &enemyInfo );

	EnemyAILastBoss(void);
	virtual ~EnemyAILastBoss(void);

	// ���삷�闼��N���X
	LastBossRight	*m_pRightHand;
	LastBossLeft	*m_pLeftHand;

	// �ӂ�ӂ핂���̂ŁA�`��Ƃ͕ʂɊ�ƂȂ�ʒu������
	math::Vector2 m_moveBasicPos;

};


/* ====================================================================== */
/**
 * @brief  ���X�{�X�E�荶��N���X
 *
 * @note
 *		
 */
/* ====================================================================== */
class LastBossHand : public TaskUnit, public Collision2DUnit
{
public:

	enum ACTION_KIND{
		ACTION_NONE,	// �������Ȃ�(��ʒu��)
		ACTION_FIST,	// �p���`
		ACTION_GUARD,	// �{�̂��K�[�h

		ACTION_MAX,
	};

	void SetBasicPos( math::Vector2 basicPos );

	// ���S���Ƀ{�XAI����Ă΂��
	void StartDie(){ TaskStartDie(); }

protected:

	LastBossHand( const std::string &readFileName, const math::Vector2 &enemyPos );
	virtual ~LastBossHand(void);

	virtual bool Init() override{return true;}
	virtual void Update() override;			// �ړ����̓������l�̍X�V
	virtual void DrawUpdate() override;		// �`��X�V
	virtual bool DieMain() override{return true;}

private:

	// ��_�ƂȂ�ʒu���
	math::Vector2	m_basicPos;

	ACTION_KIND		m_currAction;
	ACTION_KIND		m_nextAction;
};

class LastBossRight : public LastBossHand
{
public:
	
	static LastBossRight *Create( const std::string &readFileName, const math::Vector2 &enemyPos );

protected:
	
	LastBossRight( const std::string &readFileName, const math::Vector2 &enemyPos );

	// �N���X�̓G�^�C�v
	virtual const Common::TYPE_OBJECT GetTypeObject() const override{ return Common::TYPE_LAST_BOSS_LEFT; }

private:

	virtual ~LastBossRight(void);

};

class LastBossLeft : public LastBossHand
{
public:

	static LastBossLeft *Create( const std::string &readFileName, const math::Vector2 &enemyPos );

protected:
	
	LastBossLeft( const std::string &readFileName, const math::Vector2 &enemyPos );

	// �N���X�̓G�^�C�v
	virtual const Common::TYPE_OBJECT GetTypeObject() const override{ return Common::TYPE_LAST_BOSS_RIGHT; }
	
private:

	virtual ~LastBossLeft(void);
};

#endif