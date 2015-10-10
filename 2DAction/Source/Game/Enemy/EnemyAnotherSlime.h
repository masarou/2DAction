/* ====================================================================== */
/**
 * @brief  �G�Ǘ��N���X
 *
 * @note
 *		
 */
/* ====================================================================== */

#ifndef __ENEMY_ANOTHER_SLIME__
#define __ENEMY_ANOTHER_SLIME__

#include "EnemyBase.h"

class EnemyAnotherSlime : public EnemyBase
{
public:

	static EnemyAnotherSlime *Create( const uint32_t &uniqueID, const uint32_t &enemyLevel, const math::Vector2 &enemyPos );
	virtual const uint32_t GetPlayerHitDamage() const override;	// �v���C���[�Փˎ��̃_���[�W

protected:
	
	virtual bool InitMain() override;									// �h����ł̏�����
	virtual void EnemyDeath() override;									// ���S���ɌĂ΂��N���X
	virtual const Common::TYPE_OBJECT GetTypeObject() const override{ return Common::TYPE_ENEMY_SLIME_ANOTHER;}
	virtual const uint32_t	GetEnemyDefaultHP() const override;			// HP�擾
	virtual const float		GetEnemyDefaultSPD() const override;			// SPD�擾
	virtual void  ReduceDamage( Common::CMN_EVENT &eventId ) override;	// �G�̎�ށA���x�����Ń_���[�W�y������

private:

	EnemyAnotherSlime( const std::string &fileName, const uint32_t &uniqueID, const uint32_t &enemyLevel, const math::Vector2 &enemyPos );
	~EnemyAnotherSlime(void);

};

#endif // __ENEMY_ANOTHER_SLIME__