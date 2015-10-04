/* ====================================================================== */
/**
 * @brief  敵管理クラス
 *
 * @note
 *		
 */
/* ====================================================================== */

#ifndef __ENEMY_DRAGON__
#define __ENEMY_DRAGON__

#include "EnemyBase.h"

class EnemyDragon : public EnemyBase
{
public:

	static EnemyDragon *Create( const uint32_t &uniqueID );
	virtual const uint32_t GetPlayerHitDamage() const override{return 25;}	// プレイヤー衝突時のダメージ

	// 斬撃ダメージ取得
	uint32_t GetSlashingDamage() const;

protected:
	
	// クラスの敵タイプ
	virtual const Common::TYPE_OBJECT GetTypeObject() const override{ return Common::TYPE_DRAGON; }

	// 敵クラスのデフォルト値取得
	virtual const uint32_t	GetEnemyDefaultHP() const override{return 12000;}
	virtual const float		GetEnemyDefaultSPD() const override{return 1;}
	virtual const Common::ENEMY_AI GetEnemyDefaultAI() const{return Common::AI_ATTACK_DRAGON;}

	// 斬撃を受けた時の処理
	virtual void HitPlayreSlashing( const uint32_t &damageValue );

	// HPがなくなり倒されたときに呼ばれる
	virtual void  EnemyDeath() override;

	// 敵の種類、レベル等でダメージ軽減処理
	virtual void  ReduceDamage( Common::CMN_EVENT &eventId ) override;

private:

	EnemyDragon( const uint32_t &uniqueID );
	~EnemyDragon(void);

};

#endif //ENEMY_AAA