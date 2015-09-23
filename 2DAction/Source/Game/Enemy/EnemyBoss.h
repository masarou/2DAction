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

protected:
	
	virtual const Common::TYPE_OBJECT GetTypeObject() const override{ return Common::TYPE_EVENMY_BOSS; }
	virtual const uint32_t GetEnemyDefaultHP() const override{return 15000;}	// �G�N���X�̃f�t�H���gHP�擾
	virtual const uint32_t GetEnemyDefaultSPD() const override{return 1;}	// �G�N���X�̃f�t�H���gSPD�擾

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
	
	virtual const Common::TYPE_OBJECT GetTypeObject() const override{ return Common::TYPE_EVENMY_SLIME_KING; }
	virtual const uint32_t GetEnemyDefaultHP() const override{return 5000;}	// �G�N���X�̃f�t�H���gHP�擾
	virtual const uint32_t GetEnemyDefaultSPD() const;	// �G�N���X�̃f�t�H���gSPD�擾
	virtual const Common::ENEMY_AI GetEnemyDefaultAI() const{return Common::AI_SEARCHING_SLIME_KING;}
	virtual void  EnemyDeath() override; // HP���Ȃ��Ȃ�|���ꂽ�Ƃ��ɌĂ΂��


private:

	EnemySlimeKing( const uint32_t &uniqueID );
	~EnemySlimeKing(void);

};

#endif //ENEMY_AAA