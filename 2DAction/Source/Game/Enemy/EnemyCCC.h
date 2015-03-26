/* ====================================================================== */
/**
 * @brief  敵管理クラス
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
	
	virtual bool InitMain() override;		// 派生先での初期化
	virtual uint32_t GetEnemyDefaultHP() const override{return 1000;}	// 敵クラスのデフォルトHP取得

private:

	EnemyCCC( const uint32_t &uniqueID );
	~EnemyCCC(void);

};

#endif ENEMY_AAA