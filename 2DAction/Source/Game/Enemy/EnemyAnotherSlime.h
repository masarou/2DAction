/* ====================================================================== */
/**
 * @brief  敵管理クラス
 *
 * @note
 *		
 */
/* ====================================================================== */

#ifndef __ENEMY_ANOTHER_SLIME__
#define __ENEMY_ANOTHER_SLIME__

#include "EnemyBase.h"

class EnemyAnotherSlime : public EnemyBase
{
public:

	static EnemyAnotherSlime *Create( const uint32_t &uniqueID, const uint32_t &enemyLevel, const math::Vector2 &enemyPos );
	virtual const uint32_t GetPlayerHitDamage() const override;	// プレイヤー衝突時のダメージ

protected:
	
	virtual bool InitMain() override;									// 派生先での初期化
	virtual void EnemyDeath() override;									// 死亡時に呼ばれるクラス
	virtual const Common::TYPE_OBJECT GetTypeObject() const override{ return Common::TYPE_ENEMY_SLIME_ANOTHER;}
	virtual const uint32_t	GetEnemyDefaultHP() const override;			// HP取得
	virtual const float		GetEnemyDefaultSPD() const override;			// SPD取得
	virtual void  ReduceDamage( Common::CMN_EVENT &eventId ) override;	// 敵の種類、レベル等でダメージ軽減処理

private:

	EnemyAnotherSlime( const std::string &fileName, const uint32_t &uniqueID, const uint32_t &enemyLevel, const math::Vector2 &enemyPos );
	~EnemyAnotherSlime(void);

};

#endif // __ENEMY_ANOTHER_SLIME__