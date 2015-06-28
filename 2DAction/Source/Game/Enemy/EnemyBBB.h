/* ====================================================================== */
/**
 * @brief  敵管理クラス
 *
 * @note
 *		
 */
/* ====================================================================== */

#ifndef __ENEMY_BBB__
#define __ENEMY_BBB__

#include "EnemyBase.h"

class AttackGun;

class EnemyBBB : public EnemyBase
{
public:

	static EnemyBBB *Create( const uint32_t &enemyLevel, const uint32_t &uniqueID );

protected:
	
	virtual bool InitMain() override;		// 派生先での初期化
	virtual const Common::TYPE_OBJECT GetTypeObject() const override{ return Common::TYPE_EVENMY_BBB;}
	void EventUpdate( const Common::CMN_EVENT &eventId ) override;
	virtual uint32_t GetEnemyDefaultHP() const override;	// デフォルトHP取得
	virtual uint32_t GetEnemyDefaultSPD() const override;	// デフォルトSPD取得

private:

	EnemyBBB( const uint32_t &enemyLevel, const uint32_t &uniqueID );
	~EnemyBBB(void);

};

#endif ENEMY_BBB