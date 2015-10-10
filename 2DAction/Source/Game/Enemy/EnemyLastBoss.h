/* ====================================================================== */
/**
 * @brief  敵管理クラス
 *
 * @note
 *		
 */
/* ====================================================================== */

#ifndef __ENEMY_LAST_BOSS__
#define __ENEMY_LAST_BOSS__

#include "EnemyBase.h"

class LastBossRight;
class LastBossLeft;

class LastBoss : public EnemyBase
{
public:

	static LastBoss *Create( const uint32_t &uniqueID, const uint32_t &enemyLevel, const math::Vector2 &enemyPos );
	virtual const uint32_t GetPlayerHitDamage() const override{return 25;}	// プレイヤー衝突時のダメージ

protected:
	
	virtual bool InitMain();

	// クラスの敵タイプ
	virtual const Common::TYPE_OBJECT GetTypeObject() const override{ return Common::TYPE_LAST_BOSS; }

	// 敵クラスのデフォルト値取得
	virtual const uint32_t	GetEnemyDefaultHP() const override;
	virtual const float		GetEnemyDefaultSPD() const override{return 1;}
	virtual const Common::ENEMY_AI GetEnemyDefaultAI() const{return Common::AI_ATTACK_LAST_BOSS;}

	// 斬撃を受けた時の処理
	virtual void HitPlayreSlashing( const uint32_t &damageValue );

	// HPがなくなり倒されたときに呼ばれる
	virtual void  EnemyDeath() override;

	// 敵の種類、レベル等でダメージ軽減処理
	virtual void  ReduceDamage( Common::CMN_EVENT &eventId ) override;

private:

	LastBoss( const uint32_t &uniqueID, const uint32_t &enemyLevel, const math::Vector2 &enemyPos );
	~LastBoss(void);

};

#endif //__ENEMY_LAST_BOSS__