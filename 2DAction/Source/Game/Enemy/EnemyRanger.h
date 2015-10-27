/* ====================================================================== */
/**
 * @brief  敵管理クラス
 *
 * @note
 *		
 */
/* ====================================================================== */

#ifndef __ENEMY_RANGER__
#define __ENEMY_RANGER__

#include "EnemyBase.h"

class EnemyRanger : public EnemyBase
{
public:

	static EnemyRanger *Create( const uint32_t &uniqueID, const uint32_t &enemyLevel, const math::Vector2 &enemyPos );
	virtual const uint32_t GetPlayerHitDamage() const override{return 25;}	// プレイヤー衝突時のダメージ

	// 斬撃ダメージ取得
	uint32_t GetSlashingDamage() const;

protected:
	
	// クラスの敵タイプ
	virtual const Common::TYPE_OBJECT GetTypeObject() const override{ return Common::TYPE_ENEMY_RANGER; }

	// 敵クラスのデフォルト値取得
	virtual const uint32_t	GetEnemyDefaultHP() const override;
	virtual const float		GetEnemyDefaultSPD() const override{return 1;}
	virtual const Common::ENEMY_AI GetEnemyDefaultAI() const{return Common::AI_ATTACK_NEAR;}

	// 斬撃を受けた時の処理
	virtual void HitPlayreSlashing( const uint32_t &damageValue );

	// HPがなくなり倒されたときに呼ばれる
	virtual void  EnemyDeath() override;

	// 敵の種類、レベル等でダメージ軽減処理
	virtual void  ReduceDamage( Common::CMN_EVENT &eventId ) override;

private:

	EnemyRanger( const uint32_t &uniqueID, const uint32_t &enemyLevel, const math::Vector2 &enemyPos );
	~EnemyRanger(void);

};

#endif //__ENEMY_RANGER__