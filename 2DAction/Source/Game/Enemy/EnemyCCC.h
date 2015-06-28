/* ====================================================================== */
/**
 * @brief  �G�Ǘ��N���X
 *
 * @note
 *		
 */
/* ====================================================================== */

#ifndef __ENEMY_CCC__
#define __ENEMY_CCC__

#include "EnemyBase.h"

class EnemyCCC : public EnemyBase
{
public:

	static EnemyCCC *Create( const uint32_t &enemyLevel, const uint32_t &uniqueID );

protected:
	
	virtual bool InitMain() override;		// �h����ł̏�����
	virtual const Common::TYPE_OBJECT GetTypeObject() const override{ return Common::TYPE_EVENMY_CCC;}
	virtual uint32_t GetEnemyDefaultHP() const override;	// �f�t�H���gHP�擾
	virtual uint32_t GetEnemyDefaultSPD() const override;	// �f�t�H���gSPD�擾

private:

	EnemyCCC( const uint32_t &enemyLevel, const uint32_t &uniqueID );
	~EnemyCCC(void);

};

#endif ENEMY_AAA