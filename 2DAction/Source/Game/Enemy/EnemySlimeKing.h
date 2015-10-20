/* ====================================================================== */
/**
 * @brief  敵管理クラス
 *
 * @note
 *		
 */
/* ====================================================================== */

#ifndef __ENEMY_SLIME_KING__
#define __ENEMY_SLIME_KING__

#include "EnemyBase.h"

class EnemySlimeKing : public EnemyBase
{
public:

	static EnemySlimeKing *Create( const uint32_t &uniqueID, const uint32_t &enemyLevel, const math::Vector2 &enemyPos );
	virtual const uint32_t GetPlayerHitDamage() const override{return 20;}	// プレイヤー衝突時のダメージ

protected:
	
	// クラスの敵タイプ
	virtual const Common::TYPE_OBJECT GetTypeObject() const override{ return Common::TYPE_ENEMY_SLIME_KING; }

	// 敵クラスのデフォルト値取得
	virtual const uint32_t	GetEnemyDefaultHP() const override;
	virtual const float		GetEnemyDefaultSPD() const;
	virtual const Common::ENEMY_AI GetEnemyDefaultAI() const{return Common::AI_SEARCHING_SLIME_KING;}

	// HPがなくなり倒されたときに呼ばれる
	virtual void  EnemyDeath() override;

	// 敵の種類、レベル等でダメージ軽減処理
	virtual void  ReduceDamage( Common::CMN_EVENT &eventId ) override;
	
private:

	EnemySlimeKing( const uint32_t &uniqueID, const uint32_t &enemyLevel, const math::Vector2 &enemyPos );
	~EnemySlimeKing(void);

};

#endif //__ENEMY_SLIME_KING__