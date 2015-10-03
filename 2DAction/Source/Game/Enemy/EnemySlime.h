/* ====================================================================== */
/**
 * @brief  �G�Ǘ��N���X
 *
 * @note
 *		
 */
/* ====================================================================== */

#ifndef __ENEMY_AAA__
#define __ENEMY_AAA__

#include "EnemyBase.h"

class EnemySlime : public EnemyBase
{
public:

	static EnemySlime *Create( const uint32_t &enemyLevel, const uint32_t &uniqueID, const math::Vector2 &enemyPos );
	virtual const uint32_t GetPlayerHitDamage() const override;	// �v���C���[�Փˎ��̃_���[�W

protected:
	
	virtual bool InitMain() override;									// �h����ł̏�����
	virtual const Common::TYPE_OBJECT GetTypeObject() const override{ return Common::TYPE_ENEMY_SLIME;}
	virtual const uint32_t	GetEnemyDefaultHP() const override;			// HP�擾
	virtual const float		GetEnemyDefaultSPD() const override;			// SPD�擾
	virtual void  ReduceDamage( Common::CMN_EVENT &eventId ) override;	// �G�̎�ށA���x�����Ń_���[�W�y������

private:

	EnemySlime( const std::string &fileName, const uint32_t &enemyLevel, const uint32_t &uniqueID, const math::Vector2 &enemyPos );
	~EnemySlime(void);

};

#endif ENEMY_AAA