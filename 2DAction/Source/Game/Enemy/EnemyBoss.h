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
	virtual const uint32_t GetEnemyDefaultHP() const override{return 120000;}	// 敵クラスのデフォルトHP取得
	virtual const uint32_t GetEnemyDefaultSPD() const{return 1;}	// 敵クラスのデフォルトSPD取得

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
	virtual const uint32_t GetEnemyDefaultHP() const override{return 4000;}	// 敵クラスのデフォルトHP取得
	virtual const uint32_t GetEnemyDefaultSPD() const;	// 敵クラスのデフォルトSPD取得
	virtual const Common::ENEMY_AI GetEnemyDefaultAI() const{return Common::AI_SEARCHING_SLIME_KING;}

	virtual void  EnemyDeath() override; // HPがなくなり倒されたときに呼ばれる


private:

	EnemySlimeKing( const uint32_t &uniqueID );
	~EnemySlimeKing(void);

};

#endif //ENEMY_AAA