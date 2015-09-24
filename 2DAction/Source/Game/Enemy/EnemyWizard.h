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

	static EnemyWizard *Create( const uint32_t &enemyLevel, const uint32_t &uniqueID, const math::Vector2 &enemyPos );
	virtual const uint32_t GetPlayerHitDamage() const override;	// �v���C���[�Փˎ��̃_���[�W

	math::Vector2 GetCrystalPosFirst() const;
	math::Vector2 GetCrystalPosSecond() const;

protected:
	
	virtual bool InitMain() override;									// �h����ł̏�����
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

	WizardCrystal	*m_pCrystal;
	WizardCrystal	*m_pCrystalSec;
};


class WizardCrystal : public TaskUnit, public Collision2DUnit
{
public:

	static WizardCrystal *Create();
	
	virtual const Common::TYPE_OBJECT GetTypeObject() const{ return Common::TYPE_WIZARD_CRYSTAL; };

	//
	const math::Vector2 GetPos() const;
	void SetPos( const math::Vector2 &centerPos );

	void TaskDie(){ TaskStartDie(); }

protected:

	virtual void Update() override;			// �ړ����̓������l�̍X�V
	virtual void DrawUpdate() override;		// �`��X�V

private:

	WizardCrystal();
	~WizardCrystal();


};

#endif //__ENEMY_WIZARD__