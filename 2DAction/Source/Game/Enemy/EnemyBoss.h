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
	virtual const uint32_t GetPlayerHitDamage() const override{return 25;}	// プレイヤー衝突時のダメージ

	// 斬撃ダメージ取得
	uint32_t GetSlashingDamage() const;

protected:
	
	// クラスの敵タイプ
	virtual const Common::TYPE_OBJECT GetTypeObject() const override{ return Common::TYPE_ENEMY_BOSS; }

	// 敵クラスのデフォルト値取得
	virtual const uint32_t	GetEnemyDefaultHP() const override{return 12000;}
	virtual const float		GetEnemyDefaultSPD() const override{return 1;}
	virtual const Common::ENEMY_AI GetEnemyDefaultAI() const{return Common::AI_ATTACK_NEAR;}

	// 斬撃を受けた時の処理
	virtual void HitPlayreSlashing( const uint32_t &damageValue );

	// HPがなくなり倒されたときに呼ばれる
	virtual void  EnemyDeath() override;

	// 敵の種類、レベル等でダメージ軽減処理
	virtual void  ReduceDamage( Common::CMN_EVENT &eventId ) override;

private:

	EnemyBoss( const uint32_t &uniqueID );
	~EnemyBoss(void);

};




class EnemySlimeKing : public EnemyBase
{
public:

	static EnemySlimeKing *Create( const uint32_t &uniqueID );
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

	EnemySlimeKing( const uint32_t &uniqueID );
	~EnemySlimeKing(void);

};

#endif //ENEMY_AAA