/* ====================================================================== */
/**
 * @brief  �G�Ǘ��N���X
 *
 * @note
 *		
 */
/* ====================================================================== */

#ifndef __ENEMY_SLIME_KING__
#define __ENEMY_SLIME_KING__

#include "EnemyBase.h"

class EnemySlimeKing : public EnemyBase
{
public:

	static EnemySlimeKing *Create( const uint32_t &uniqueID, const uint32_t &enemyLevel, const math::Vector2 &enemyPos );
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

	EnemySlimeKing( const uint32_t &uniqueID, const uint32_t &enemyLevel, const math::Vector2 &enemyPos );
	~EnemySlimeKing(void);

};

#endif //__ENEMY_SLIME_KING__