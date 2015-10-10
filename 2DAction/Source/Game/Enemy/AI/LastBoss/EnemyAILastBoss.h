/* ====================================================================== */
/**
 * @brief  ラスボスAI
 *
 * @note
 *		
 */
/* ====================================================================== */

#ifndef __ENEMY_AI_LAST_BOSS__
#define __ENEMY_AI_LAST_BOSS__

#include "Game/Enemy/AI/EnemyAIBase.h"

class LastBossRight;
class LastBossLeft;

class EnemyAILastBoss : public EnemyAIBase
{
public:

	static EnemyAILastBoss *Create();

	// AIの種類を派生先でセットしておく
	virtual const Common::ENEMY_AI GetAIKind() const{ return Common::AI_ATTACK_LAST_BOSS; }

protected:
	
	virtual bool InitAI() override;								// AI初期化
	virtual void ExecMain( TEX_DRAW_INFO &enemyInfo, ACTION_ARRAY &actionInfo ) override;	// 派生先でのAI実装
	virtual void EnemyIsDead();

private:

	void ExecHandsUpdate( TEX_DRAW_INFO &enemyInfo );

	EnemyAILastBoss(void);
	virtual ~EnemyAILastBoss(void);

	// 操作する両手クラス
	LastBossRight	*m_pRightHand;
	LastBossLeft	*m_pLeftHand;

	// ふわふわ浮くので、描画とは別に基準となる位置を持つ
	math::Vector2 m_moveBasicPos;

};


/* ====================================================================== */
/**
 * @brief  ラスボス右手左手クラス
 *
 * @note
 *		
 */
/* ====================================================================== */
class LastBossHand : public TaskUnit, public Collision2DUnit
{
public:

	enum ACTION_KIND{
		ACTION_NONE,	// 何もしない(定位置へ)
		ACTION_FIST,	// パンチ
		ACTION_GUARD,	// 本体をガード

		ACTION_MAX,
	};

	void SetBasicPos( math::Vector2 basicPos );

	// 死亡時にボスAIから呼ばれる
	void StartDie(){ TaskStartDie(); }

protected:

	LastBossHand( const std::string &readFileName, const math::Vector2 &enemyPos );
	virtual ~LastBossHand(void);

	virtual bool Init() override{return true;}
	virtual void Update() override;			// 移動等の内部数値の更新
	virtual void DrawUpdate() override;		// 描画更新
	virtual bool DieMain() override{return true;}

private:

	// 基準点となる位置情報
	math::Vector2	m_basicPos;

	ACTION_KIND		m_currAction;
	ACTION_KIND		m_nextAction;
};

class LastBossRight : public LastBossHand
{
public:
	
	static LastBossRight *Create( const std::string &readFileName, const math::Vector2 &enemyPos );

protected:
	
	LastBossRight( const std::string &readFileName, const math::Vector2 &enemyPos );

	// クラスの敵タイプ
	virtual const Common::TYPE_OBJECT GetTypeObject() const override{ return Common::TYPE_LAST_BOSS_LEFT; }

private:

	virtual ~LastBossRight(void);

};

class LastBossLeft : public LastBossHand
{
public:

	static LastBossLeft *Create( const std::string &readFileName, const math::Vector2 &enemyPos );

protected:
	
	LastBossLeft( const std::string &readFileName, const math::Vector2 &enemyPos );

	// クラスの敵タイプ
	virtual const Common::TYPE_OBJECT GetTypeObject() const override{ return Common::TYPE_LAST_BOSS_RIGHT; }
	
private:

	virtual ~LastBossLeft(void);
};

#endif