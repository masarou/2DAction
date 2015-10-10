/* ====================================================================== */
/**
 * @brief  �G�Ǘ��N���X
 *
 * @note
 *		
 */
/* ====================================================================== */

#ifndef __ENEMY_WIZARD__
#define __ENEMY_WIZARD__

#include "EnemyBase.h"
#include "System/Task/SystemTaskUnit.h"
#include "System/Collision/SystemCollisionUnit.h"

class WizardCrystal;

class EnemyWizard : public EnemyBase
{

public:

	// �N���X�^���̉�]�̒��S�ƂȂ�I�u�W�F�N�g���
	enum CRYSTAL_AROUND{
		CRYSTAL_AROUND_MINE,	// �N���X�^���������̎���ŉ�
		CRYSTAL_AROUND_PLAYER,	// �N���X�^�����v���C���̎���ŉ�

		CRYSTAL_AROUND_MAX,
	};

	// ���ݗL���ȃ_���[�W���
	enum DAMAGE_TYPE{
		DAMAGE_TYPE_SLASH,	// �a��
		DAMAGE_TYPE_BULLET,	// �}�V���K��

		DAMAGE_TYPE_MAX,
	};

	static EnemyWizard *Create( const uint32_t &uniqueID, const uint32_t &enemyLevel, const math::Vector2 &enemyPos );
	virtual const uint32_t GetPlayerHitDamage() const override;	// �v���C���[�Փˎ��̃_���[�W

	const uint32_t GetCrystalIndex() const{ return m_pCrystalVec.size(); };	// �����N���X�^�����擾
	const math::Vector2 GetCrystalPos( const uint32_t &index) const;		// �w��Index�̃N���X�^���ʒu�擾

	// �}�V���K���_���[�W�擾
	uint32_t GetBulletDamage() const;

	// �N���X�^���̒��S�Ώێ擾�Z�b�g
	const CRYSTAL_AROUND GetCrystalAroundTarget() const{ return m_crystalAround; }
	void SetCrystalAroundTarget( const CRYSTAL_AROUND &type );

	// �N���X�^���̉�]���a�Z�b�g
	void SetCrystalAroundDistance( const uint32_t &distance );

	// �_���[�W�^�C�v�擾�Z�b�g
	const DAMAGE_TYPE GetCurrentDamageType() const{ return m_damageType; }
	void SetCurrentDamageType( const DAMAGE_TYPE &type ){ m_damageType = type; }

protected:
	
	virtual bool InitMain() override;			// �h����ł̏�����
	virtual void UpdateCustom() override;		// �h����ł̍X�V����
	virtual bool DieMainCustom();				// �h����ł̎��S������
	
	// �N���X�̓G�^�C�v
	virtual const Common::TYPE_OBJECT GetTypeObject() const override{ return Common::TYPE_ENEMY_WIZARD; }

	// �G�N���X�̃f�t�H���g�l�擾
	virtual const uint32_t	GetEnemyDefaultHP() const override;
	virtual const float		GetEnemyDefaultSPD() const override;
	virtual const Common::ENEMY_AI GetEnemyDefaultAI() const{return Common::AI_ATTACK_WIZARD;}	// �G�N���X�̃f�t�H���gSPD�擾

	// �G�̎�ށA���x�����Ń_���[�W�y������
	virtual void  ReduceDamage( Common::CMN_EVENT &eventId ) override;
	virtual void HitPlayreSlashing( const uint32_t &damageValue ) override;
	
	// HP���Ȃ��Ȃ�|���ꂽ�Ƃ��ɌĂ΂��
	virtual void  EnemyDeath() override;


private:

	EnemyWizard( const std::string &fileName, const uint32_t &uniqueID, const uint32_t &enemyLevel, const math::Vector2 &enemyPos );
	~EnemyWizard(void);

	CRYSTAL_AROUND					m_crystalAround;	// �N���X�^���������S�ɉ���Ă��邩
	DAMAGE_TYPE						m_damageType;		// �L���ȃ_���[�W���
	std::vector< WizardCrystal* >	m_pCrystalVec;		// �N���X�^���N���X
};


class WizardCrystal : public TaskUnit, public Collision2DUnit
{
public:

	static WizardCrystal *Create( uint32_t startRot );
	
	virtual const Common::TYPE_OBJECT GetTypeObject() const{ return Common::TYPE_WIZARD_CRYSTAL; };

	// �e��p�����[�^�ύX
	const math::Vector2 GetPos() const;
	void SetPos( const math::Vector2 &centerPos );
	void SetRadius( const uint32_t &newRadius ){ m_circleRadius = newRadius; }

	void TaskDie(){ TaskStartDie(); }

protected:

	virtual void Update() override;			// �ړ����̓������l�̍X�V
	virtual void DrawUpdate() override;		// �`��X�V

private:

	WizardCrystal( uint32_t startRot );
	~WizardCrystal();

	uint32_t	m_startRot;		// ��]�̊J�n�p�x
	uint32_t	m_rotCounter;	// ��]�J�E���^
	uint32_t	m_circleRadius;	// ��]�~�̔��a

};

#endif //__ENEMY_WIZARD__