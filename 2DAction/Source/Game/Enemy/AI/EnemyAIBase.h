/* ====================================================================== */
/**
 * @brief  敵AIのBaseクラス
 *
 * @note
 *		
 */
/* ====================================================================== */

#ifndef __ENEMY_AI_BASE__
#define __ENEMY_AI_BASE__

#include "Game/Enemy/AI/EnemyAIBase.h"
#include "Common/CommonDefine.h"
#include "System/Draw2D/SystemDraw2DDefine.h"
#include "Game/Attack/GameAttackBlade.h"

#include "EnemyAIDefine.h"

class EnemyBase;
class AttackGun;

// 共通での距離の認識
static const float DISTANCE_TO_PLAYER_SIDE		= 200.0f;
static const float DISTANCE_TO_PLAYER_NEAR		= 300.0f;
static const float DISTANCE_TO_PLAYER_MIDDLE	= 600.0f;
static const float DISTANCE_TO_PLAYER_FAR		= 1000.0f;

class EnemyAIBase
{

public:

	virtual ~EnemyAIBase(void);

	void SetThingingEnemy( EnemyBase *enemyMine );
	const bool &IsReady(){ return m_isReady; }	// 初期化準備完了かどうか
	void Exec( TEX_DRAW_INFO &enemyInfo, ACTION_ARRAY &actionInfo );		// 思考開始

	// AIの種類を派生先でセットしておく
	virtual const Common::ENEMY_AI GetAIKind() const = 0 ;

	// 敵キャラそのものに関するget,set関数
	void SetEnemyAnim( const std::string &animTag );
	std::string GetEnemyAnim();
	void SetEnemyEyeSight( math::Vector2 eye );
	math::Vector2 GetEnemyEyeSight() const;
	void ChangeEnemyAI( Common::ENEMY_AI nextAI );
	Common::ENEMY_KIND GetEnemyKind() const;
	const std::string &GetEnemyJsonName() const;
	const uint32_t GetEnemyLevel() const;
	const float GetEnemySPD() const;
	const math::Vector2 GetEnemyPos() const;

	// 敵クラス取得
	EnemyBase *UpdateEnemyMine();
	const EnemyBase *const GetEnemeyMine() const;

	// 攻撃クラス解放
	static void ClearAttackMaterial();

	// このAIになってからの経過時間を取得( 1000 = 1s )
	uint32_t GetTimeStartThisAIBySec();
	
	// 敵クラスが他のクラスからもらったイベント
	virtual void EnemyRecievedEvent( const Common::CMN_EVENT &eventInfo ){};

	// 敵クラス死亡時に呼ばれる
	virtual void EnemyIsDead(){};

protected:
	
	EnemyAIBase();

	virtual bool InitAI(){ return true; }					// AI初期化
	virtual void ExecMain( TEX_DRAW_INFO &enemyInfo, ACTION_ARRAY &actionInfo ) = 0;	// 派生先でのAI実装

	// 以下、必要になるであろう便利関数
	void ShootBullet( const math::Vector2 &pos = math::Vector2(), const math::Vector2 &vec = math::Vector2(), const uint32_t &damage = 20.0f, const uint32_t &speed = 10.0f );	// 攻撃弾生成
	void Slashing( const Slashing::TYPE_SLASHING &type, const uint32_t &damageValue, const math::Vector2 &vec = math::Vector2() );

	// すべての敵クラスで共有
	static AttackGun	*s_pAttackGun;		// マシンガンクラス
	static AttackBlade	*s_pAttackBlade;	// 斬撃クラス

private:
	
	uint32_t	m_thisAIStartTime;	// このAIに切り替わってからの時間
	bool		m_isReady;			// 思考準備が終了したかどうか
	EnemyBase	*m_enemyMine;		// 現在の自分の状態を知るために保持しておく
};

#endif