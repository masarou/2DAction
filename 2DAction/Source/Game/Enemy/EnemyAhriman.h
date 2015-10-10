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

	static EnemyBBB *Create( const uint32_t &uniqueID, const uint32_t &enemyLevel, const math::Vector2 &enemyPos );
	virtual const uint32_t GetPlayerHitDamage() const override;	// プレイヤー衝突時のダメージ

protected:
	
	virtual bool InitMain() override;		// 派生先での初期化
	virtual const Common::TYPE_OBJECT GetTypeObject() const override{ return Common::TYPE_ENEMY_AHRIMAN;}
	void EventUpdate( Common::CMN_EVENT &eventId ) override;
	virtual const uint32_t	GetEnemyDefaultHP() const override;	// デフォルトHP取得
	virtual const float		GetEnemyDefaultSPD() const override;	// デフォルトSPD取得

private:

	EnemyBBB( const uint32_t &uniqueID, const uint32_t &enemyLevel, const math::Vector2 &enemyPos );
	~EnemyBBB(void);

};

#endif ENEMY_BBB