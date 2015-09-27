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

	static EnemyWizard *Create( const uint32_t &enemyLevel, const uint32_t &uniqueID, const math::Vector2 &enemyPos );
	virtual const uint32_t GetPlayerHitDamage() const override;	// �v���C���[�Փˎ��̃_���[�W

	const uint32_t GetCrystalIndex() const{ return m_pCrystalVec.size(); };
	const math::Vector2 GetCrystalPos( const uint32_t &index) const;

	// �}�V���K���_���[�W�擾
	uint32_t GetBulletDamage() const;

	// �N���X�^���̒��S�Ώێ擾�Z�b�g
	const CRYSTAL_AROUND GetCrystalAroundTarget() const{ return m_crystalAround; }
	void SetCrystalAroundTarget( const CRYSTAL_AROUND &type );

	// �_���[�W�^�C�v�擾�Z�b�g
	const DAMAGE_TYPE GetCurrentDamageType() const{ return m_damageType; }
	void SetCurrentDamageType( const DAMAGE_TYPE &type ){ m_damageType = type; }

protected:
	
	virtual bool InitMain() override;									// �h����ł̏�����
	virtual void UpdateCustom() override;								// �h����ł̍X�V����
	virtual const Common::TYPE_OBJECT GetTypeObject() const override{ return Common::TYPE_ENEMY_WIZARD; }
	virtual const uint32_t GetEnemyDefaultHP() const override;			// HP�擾
	virtual const uint32_t GetEnemyDefaultSPD() const override;			// SPD�擾
	virtual void  ReduceDamage( Common::CMN_EVENT &eventId ) override;	// �G�̎�ށA���x�����Ń_���[�W�y������
	virtual const Common::ENEMY_AI GetEnemyDefaultAI() const{return Common::AI_ATTACK_WIZARD;}	// �G�N���X�̃f�t�H���gSPD�擾

	// ���̃N���X�Œ�`���鉼�z�֐�
	virtual bool DieMainCustom();

private:

	EnemyWizard( const std::string &fileName, const uint32_t &enemyLevel, const uint32_t &uniqueID, const math::Vector2 &enemyPos );
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
	void SetRadius( uint32_t &newRadius ){ m_circleRadius = newRadius; }

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