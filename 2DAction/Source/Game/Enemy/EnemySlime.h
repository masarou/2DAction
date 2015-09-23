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

class EnemySlime : public EnemyBase
{
public:

	static EnemySlime *Create( const uint32_t &enemyLevel, const uint32_t &uniqueID, const math::Vector2 &enemyPos );
	virtual const uint32_t GetPlayerHitDamage() const override;	// プレイヤー衝突時のダメージ

protected:
	
	virtual bool InitMain() override;						// 派生先での初期化
	virtual const Common::TYPE_OBJECT GetTypeObject() const override{ return Common::TYPE_EVENMY_SLIME;}
	virtual const uint32_t GetEnemyDefaultHP() const override;	// HP取得
	virtual const uint32_t GetEnemyDefaultSPD() const override;	// SPD取得

private:

	EnemySlime( const std::string &fileName, const uint32_t &enemyLevel, const uint32_t &uniqueID, const math::Vector2 &enemyPos );
	~EnemySlime(void);

};

#endif ENEMY_AAA