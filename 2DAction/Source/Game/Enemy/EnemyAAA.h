/* ====================================================================== */
/**
 * @brief  敵管理クラス
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
	
	virtual uint32_t GetEnemyDefaultHP(){return 100;}	// 敵クラスのデフォルトHP取得

private:

	EnemyAAA( const uint32_t &uniqueID );
	~EnemyAAA(void);

};

#endif ENEMY_AAA