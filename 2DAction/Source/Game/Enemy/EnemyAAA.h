/* ====================================================================== */
/**
 * @brief  �G�Ǘ��N���X
 *
 * @note
 *		
 */
/* ====================================================================== */

#ifndef ENEMY_AAA
#define ENEMY_AAA

#include "EnemyBase.h"

class EnemyAAA : public EnemyBase
{
public:

	static EnemyAAA *Create( const uint32_t &uniqueID );

protected:
	
	virtual bool InitMain() override;		// �h����ł̏�����
	virtual uint32_t GetEnemyDefaultHP() const override{return 100;}	// �G�N���X�̃f�t�H���gHP�擾

private:

	EnemyAAA( const uint32_t &uniqueID );
	~EnemyAAA(void);

};

#endif ENEMY_AAA