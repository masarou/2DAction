/* ====================================================================== */
/**
 * @brief  敵管理クラス
 *
 * @note
 *		
 */
/* ====================================================================== */

#ifndef __ENEMY_BOSS__
#define __ENEMY_BOSS__

#include "EnemyBase.h"

class EnemyBoss : public EnemyBase
{
public:

	static EnemyBoss *Create( const uint32_t &uniqueID );

protected:
	
	virtual const Common::TYPE_OBJECT GetTypeObject() const override{ return Common::TYPE_EVENMY_BOSS; }
	virtual uint32_t GetEnemyDefaultHP() const override{return 40000;}	// 敵クラスのデフォルトHP取得
	virtual uint32_t GetEnemyDefaultSPD() const{return 1;}	// 敵クラスのデフォルトSPD取得

private:

	EnemyBoss( const uint32_t &uniqueID );
	~EnemyBoss(void);

};




class EnemySlimeKing : public EnemyBase
{
public:

	static EnemySlimeKing *Create( const uint32_t &uniqueID );

protected:
	
	virtual const Common::TYPE_OBJECT GetTypeObject() const override{ return Common::TYPE_EVENMY_SLIME_KING; }
	virtual uint32_t GetEnemyDefaultHP() const override{return 40000;}	// 敵クラスのデフォルトHP取得
	virtual uint32_t GetEnemyDefaultSPD() const{return 1;}	// 敵クラスのデフォルトSPD取得

private:

	EnemySlimeKing( const uint32_t &uniqueID );
	~EnemySlimeKing(void);

};

#endif //ENEMY_AAA