/* ====================================================================== */
/**
 * @brief  敵管理クラス
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

	static EnemyAAA *Create( const uint32_t &uniqueID );

protected:
	
	virtual bool InitMain() override;						// 派生先での初期化
	virtual uint32_t GetEnemyDefaultHP() const override;	// 敵クラスのデフォルトHP取得

private:

	EnemyAAA( const uint32_t &uniqueID );
	~EnemyAAA(void);

};

#endif ENEMY_AAA