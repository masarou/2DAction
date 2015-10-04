/* ====================================================================== */
/**
 * @brief  �G�Ǘ��N���X
 *
 * @note
 *		
 */
/* ====================================================================== */

#ifndef __ENEMY_DRAGON__
#define __ENEMY_DRAGON__

#include "EnemyBase.h"

class EnemyDragon : public EnemyBase
{
public:

	static EnemyDragon *Create( const uint32_t &uniqueID );
	virtual const uint32_t GetPlayerHitDamage() const override{return 25;}	// �v���C���[�Փˎ��̃_���[�W

	// �a���_���[�W�擾
	uint32_t GetSlashingDamage() const;

protected:
	
	// �N���X�̓G�^�C�v
	virtual const Common::TYPE_OBJECT GetTypeObject() const override{ return Common::TYPE_DRAGON; }

	// �G�N���X�̃f�t�H���g�l�擾
	virtual const uint32_t	GetEnemyDefaultHP() const override{return 12000;}
	virtual const float		GetEnemyDefaultSPD() const override{return 1;}
	virtual const Common::ENEMY_AI GetEnemyDefaultAI() const{return Common::AI_ATTACK_DRAGON;}

	// �a�����󂯂����̏���
	virtual void HitPlayreSlashing( const uint32_t &damageValue );

	// HP���Ȃ��Ȃ�|���ꂽ�Ƃ��ɌĂ΂��
	virtual void  EnemyDeath() override;

	// �G�̎�ށA���x�����Ń_���[�W�y������
	virtual void  ReduceDamage( Common::CMN_EVENT &eventId ) override;

private:

	EnemyDragon( const uint32_t &uniqueID );
	~EnemyDragon(void);

};

#endif //ENEMY_AAA