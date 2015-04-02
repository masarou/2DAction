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
	const uint32_t &GetWalkHeight() const{ return m_walkHeight; }

protected:

	EnemyBase( const std::string &jsonName, const uint32_t &uniqueId, const Common::ENEMY_KIND &kind );

	virtual bool Init() override;						// 初期化
	virtual bool InitMain(){ return true; }				// 派生先での初期化
	virtual bool DieMain() override;					// 派生先での初期化
	virtual void Update() override;						// 位置やAIによる数値周りの更新
	virtual void DrawUpdate() override;					// 描画更新

	// ほかのクラスからのイベント処理
	virtual void EventUpdate( const Common::CMN_EVENT &eventId ) override;
	virtual void HitPlayreBullet( const uint32_t &damageValue );// 弾が当たった時の処理
	virtual void HitPlayreSlashing( const uint32_t &damageValue );// 斬撃が当たった時の処理

	// 派生先でセットする関数
	virtual uint32_t GetEnemyDefaultHP() const{return 10;}	// 敵クラスのデフォルトHP取得

	// このクラスの種類セット
	virtual const Common::TYPE_OBJECT GetTypeObject() const override{ return Common::TYPE_EVENMY_AAA; }

	// このクラスで定義する仮想関数
	virtual bool DieMainCustom(){ return true; }

	EnemyAIBase			*m_pEnemyAI;					// 思考クラス

private:

	// ダメージ共通処理
	void UpdateEnemyDamage( const uint32_t &damageValue );

	// AIの行動を反映
	void RefrectAIAction();

	uint32_t			m_uniqueIdOfEnemyAll;			// 敵全体の中での識別ID
	Common::ENEMY_KIND	m_enemyKind;					// 敵の種類
	uint32_t			m_HP;							// 敵体力
	math::Vector2		m_eye;							// 敵の視線
	uint32_t			m_walkHeight;					// 敵の歩ける高さ
	uint32_t			m_stunTime;						// 何かしらの理由によって動けない時間

	Texture2D			m_textureLife;					// 敵ライフ画像

	ACTION_ARRAY		m_actionInfoAI;					// AIの思考結果格納変数
	Common::ENEMY_AI	m_nextAI;						// 次に思考するAIステート
	Common::ENEMY_AI	m_prevAI;						// ひとつ前のAIステート

};

#endif