/* ====================================================================== */
/**
 * @brief  �G�Ǘ��N���X
 *
 * @note
 *		
 */
/* ====================================================================== */

#ifndef __ENEMY_RANGER__
#define __ENEMY_RANGER__

#include "EnemyBase.h"

class EnemyRanger : public EnemyBase
{
public:

	static EnemyRanger *Create( const uint32_t &uniqueID, const uint32_t &enemyLevel, const math::Vector2 &enemyPos );
	virtual const uint32_t GetPlayerHitDamage() const override{return 25;}	// �v���C���[�Փˎ��̃_���[�W

	// �a���_���[�W�擾
	uint32_t GetSlashingDamage() const;

protected:
	
	// �N���X�̓G�^�C�v
	virtual const Common::TYPE_OBJECT GetTypeObject() const override{ return Common::TYPE_ENEMY_RANGER; }

	// �G�N���X�̃f�t�H���g�l�擾
	virtual const uint32_t	GetEnemyDefaultHP() const override;
	virtual const float		GetEnemyDefaultSPD() const override{return 1;}
	virtual const Common::ENEMY_AI GetEnemyDefaultAI() const{return Common::AI_ATTACK_NEAR;}

	// �a�����󂯂����̏���
	virtual void HitPlayreSlashing( const uint32_t &damageValue );

	// HP���Ȃ��Ȃ�|���ꂽ�Ƃ��ɌĂ΂��
	virtual void  EnemyDeath() override;

	// �G�̎�ށA���x�����Ń_���[�W�y������
	virtual void  ReduceDamage( Common::CMN_EVENT &eventId ) override;

private:

	EnemyRanger( const uint32_t &uniqueID, const uint32_t &enemyLevel, const math::Vector2 &enemyPos );
	~EnemyRanger(void);

};

#endif //__ENEMY_RANGER__