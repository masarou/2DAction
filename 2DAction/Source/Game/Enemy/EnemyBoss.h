/* ====================================================================== */
/**
 * @brief  �G�Ǘ��N���X
 *
 * @note
 *		
 */
/* ====================================================================== */

#ifndef __ENEMY_BOSS__
#define __ENEMY_BOSS__

#include "EnemyBase.h"

class EnemyBoss : public EnemyBase
{
public:

	static EnemyBoss *Create( const uint32_t &uniqueID );
	virtual const uint32_t GetPlayerHitDamage() const override{return 25;}	// �v���C���[�Փˎ��̃_���[�W

	// �a���_���[�W�擾
	uint32_t GetSlashingDamage() const;

protected:
	
	// �N���X�̓G�^�C�v
	virtual const Common::TYPE_OBJECT GetTypeObject() const override{ return Common::TYPE_ENEMY_BOSS; }

	// �G�N���X�̃f�t�H���g�l�擾
	virtual const uint32_t	GetEnemyDefaultHP() const override{return 12000;}
	virtual const float		GetEnemyDefaultSPD() const override{return 1;}
	virtual const Common::ENEMY_AI GetEnemyDefaultAI() const{return Common::AI_ATTACK_NEAR;}

	// �a�����󂯂����̏���
	virtual void HitPlayreSlashing( const uint32_t &damageValue );

	// HP���Ȃ��Ȃ�|���ꂽ�Ƃ��ɌĂ΂��
	virtual void  EnemyDeath() override;

	// �G�̎�ށA���x�����Ń_���[�W�y������
	virtual void  ReduceDamage( Common::CMN_EVENT &eventId ) override;

private:

	EnemyBoss( const uint32_t &uniqueID );
	~EnemyBoss(void);

};




class EnemySlimeKing : public EnemyBase
{
public:

	static EnemySlimeKing *Create( const uint32_t &uniqueID );
	virtual const uint32_t GetPlayerHitDamage() const override{return 20;}	// �v���C���[�Փˎ��̃_���[�W

protected:
	
	// �N���X�̓G�^�C�v
	virtual const Common::TYPE_OBJECT GetTypeObject() const override{ return Common::TYPE_ENEMY_SLIME_KING; }

	// �G�N���X�̃f�t�H���g�l�擾
	virtual const uint32_t	GetEnemyDefaultHP() const override;
	virtual const float		GetEnemyDefaultSPD() const;
	virtual const Common::ENEMY_AI GetEnemyDefaultAI() const{return Common::AI_SEARCHING_SLIME_KING;}

	// HP���Ȃ��Ȃ�|���ꂽ�Ƃ��ɌĂ΂��
	virtual void  EnemyDeath() override;

	// �G�̎�ށA���x�����Ń_���[�W�y������
	virtual void  ReduceDamage( Common::CMN_EVENT &eventId ) override;
	
private:

	EnemySlimeKing( const uint32_t &uniqueID );
	~EnemySlimeKing(void);

};

#endif //ENEMY_AAA