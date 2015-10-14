/* ====================================================================== */
/**
 * @brief  敵管理クラス
 *
 * @note
 *		
 */
/* ====================================================================== */

#ifndef __ENEMY_BASE__
#define __ENEMY_BASE__

#include "Common/CommonDefine.h"
#include "Game/GameRecorder.h"
#include "Common/Utility/CommonGameUtility.h"
#include "AI/EnemyAIDefine.h"
#include "AI/EnemyAIBase.h"
#include "EnemyManager.h"
#include "Game/Effect/GameEffect.h"
#include "System/Task/SystemTaskUnit.h"
#include "System/SystemDefine.h"
#include "System/Collision/SystemCollisionUnit.h"

class EnemyBase : public TaskUnit, public Collision2DUnit
{
	
	friend EnemyAIBase;
	friend EnemyManager;

public:

	virtual ~EnemyBase(void);

	// 情報取得関数
	const Common::ENEMY_KIND &GetKind() const{ return m_enemyKind; }
	const TEX_DRAW_INFO &GetDrawInfo() const;
	const uint32_t &GetEnemyHitPoint() const{ return m_HP; }
	const uint32_t &GetUniqueNumber() const{ return m_uniqueIdOfEnemyAll; }
	const math::Vector2 &GetEnemyEyeSight() const{ return m_eye; }
	const int32_t &GetWalkHeight() const{ return m_walkHeight; }
	const uint32_t &GetEnemyLevel() const;
	const Common::ENEMY_AI GetCurrentAIKind() const;

protected:

	EnemyBase( const std::string &jsonName, const uint32_t &uniqueId, const Common::ENEMY_KIND &kind, const uint32_t &enemyLevel, const math::Vector2 &enemyPos = DEFAULT_VECTOR2 );

	// 基本継承禁止
	virtual bool Init() override;						// 初期化
	virtual bool DieMain() override;					// 死亡前処理
	virtual void Update() override;						// 位置やAIによる数値周りの更新
	virtual void DrawUpdate() override;					// 描画更新

	// 継承先で必要な処理実装
	virtual bool InitMain(){ return true; }				// 派生先での初期化
	virtual void UpdateCustom(){}						// 派生先での更新処理
	virtual void DrawUpdateCustom(){}					// 派生先での描画処理
	virtual bool DieMainCustom(){ return true; }

	// ほかのクラスからのイベント処理
	virtual void EventUpdate( Common::CMN_EVENT &eventId ) override;
	virtual void HitPlayreBullet( const uint32_t &damageValue );// 弾が当たった時の処理
	virtual void HitPlayreSlashing( const uint32_t &damageValue );// 斬撃が当たった時の処理

	// 派生先でセットする関数
	virtual const uint32_t	GetEnemyDefaultHP() const{return 10;}		// 敵クラスのデフォルトHP取得
	virtual const float		GetEnemyDefaultSPD() const{return 1.0f;}	// 敵クラスのデフォルトSPD取得
	virtual const uint32_t	GetPlayerHitDamage() const{return 10;}		// プレイヤー衝突時のダメージ
	virtual const Common::ENEMY_AI GetEnemyDefaultAI() const{return Common::AI_SEARCHING;}	// 敵クラスのデフォルトSPD取得
	virtual void  EnemyDeath();											// HPがなくなり倒されたときに呼ばれる
	virtual void  ReduceDamage( Common::CMN_EVENT &eventId ){};			// 敵の種類、レベル等でダメージ軽減処理

	// 行動不能時間セット
	void SetStunTime( const uint32_t &stunTime ){ m_stunTime = stunTime; }

	// このクラスの種類セット
	virtual const Common::TYPE_OBJECT GetTypeObject() const override = 0;

	// ダメージ共通処理
	void UpdateEnemyDamage( const uint32_t &damageValue );

	EnemyAIBase			*m_pEnemyAI;					// 思考クラス

private:

	// AIの行動を反映
	void RefrectAIAction();

	uint32_t			m_uniqueIdOfEnemyAll;			// 敵全体の中での識別ID
	Common::ENEMY_KIND	m_enemyKind;					// 敵の種類
	uint32_t			m_enemyLv;						// 敵の強さ
	uint32_t			m_HP;							// 敵体力
	uint32_t			m_speed;						// 敵のベースとなるスピード
	math::Vector2		m_eye;							// 敵の視線
	int32_t				m_walkHeight;					// 敵の歩ける高さ
	uint32_t			m_stunTime;						// 何かしらの理由によって動けない時間

	Texture2D			m_textureLife;					// 敵ライフ画像

	ACTION_ARRAY		m_actionInfoAI;					// AIの思考結果格納変数
	Common::ENEMY_AI	m_nextAI;						// 次に思考するAIステート
	Common::ENEMY_AI	m_prevAI;						// ひとつ前のAIステート

};

#endif