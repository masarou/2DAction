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

class EnemyAAA : public EnemyBase
{
public:

	static EnemyAAA *Create( const uint32_t &enemyLevel, const uint32_t &uniqueID, const math::Vector2 &enemyPos );

protected:
	
	virtual bool InitMain() override;						// �h����ł̏�����
	virtual const Common::TYPE_OBJECT GetTypeObject() const override{ return Common::TYPE_EVENMY_AAA;}
	virtual const uint32_t GetEnemyDefaultHP() const override;	// HP�擾
	virtual const uint32_t GetEnemyDefaultSPD() const override;	// SPD�擾

private:

	EnemyAAA( const uint32_t &enemyLevel, const uint32_t &uniqueID, const math::Vector2 &enemyPos );
	~EnemyAAA(void);

};

#endif ENEMY_AAA