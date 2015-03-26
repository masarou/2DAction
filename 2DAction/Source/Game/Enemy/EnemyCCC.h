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

	static EnemyCCC *Create( const uint32_t &uniqueID );

protected:
	
	virtual bool InitMain() override;		// �h����ł̏�����
	virtual uint32_t GetEnemyDefaultHP() const override{return 1000;}	// �G�N���X�̃f�t�H���gHP�擾

private:

	EnemyCCC( const uint32_t &uniqueID );
	~EnemyCCC(void);

};

#endif ENEMY_AAA