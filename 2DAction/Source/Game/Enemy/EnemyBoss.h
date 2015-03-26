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

	// ボスが行動する際の目的地
	static math::Vector2 BOSS_TARGET_PLACE;

	static EnemyBoss *Create( const uint32_t &uniqueID );

protected:
	
	virtual bool InitMain() override;		// 派生先での初期化
	virtual uint32_t GetEnemyDefaultHP() const override{return 1000;}	// 敵クラスのデフォルトHP取得
	void EventUpdate( const Common::CMN_EVENT &eventId ) override;

private:

	EnemyBoss( const uint32_t &uniqueID );
	~EnemyBoss(void);

};

#endif //ENEMY_AAA