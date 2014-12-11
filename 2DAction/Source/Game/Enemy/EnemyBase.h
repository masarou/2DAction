/* ====================================================================== */
/**
 * @brief  敵管理クラス
 *
 * @note
 *		
 */
/* ====================================================================== */

#ifndef ENEMY_BASE
#define ENEMY_BASE

#include "Common/CommonDefine.h"
#include "System/SystemDefine.h"
#include "Game/Game2DBase.h"
#include "Game/Enemy/EnemyAIBase.h"
#include "Game/Enemy/EnemyManager.h"

class EnemyBase
{

	friend EnemyManager;

public:

	enum ENEMY_STATE{
		ENEMY_INIT,
		ENEMY_ACTION,

		ENEMY_MAX
	};

	virtual ~EnemyBase(void);

	void EventUpdate( const Common::CMN_EVENT &eventId );	// managerからのイベント処理

	// 情報取得関数
	const ENEMY_STATE &GetState() const{ return m_enemyState; }
	const TEX_DRAW_INFO &GetDrawInfo() const;
	const uint32_t &GetEnemyHitPoint() const{ return m_HP; }
	const math::Vector2 &GetEnemyEyeSight() const{ return m_eye; }
	const uint32_t &GetUniqueNumber() const{ return m_uniqueIdOfEnemyAll; }
	void ChangeAIState( const Common::ENEMY_AI &nextAI );

protected:

	EnemyBase( const std::string &jsonName, const uint32_t &uniqueId, const Common::ENEMY_KIND &kind );

	virtual bool Init();								// 初期化
	virtual void UpdateEnemy();							// 位置やAIによる数値周りの更新
	virtual void DrawEnemy();							// 描画更新
	virtual void HitPlayreBullet( uint32_t damageValue );// 弾が当たった時の処理

	// 派生先でセットする関数
	virtual uint32_t GetEnemyDefaultHP(){return 10;}	// 敵クラスのデフォルトHP取得

	EnemyAIBase			*m_pEnemyAI;					// 思考クラス

private:

	ENEMY_STATE			m_enemyState;					// このクラスの状態
	uint32_t			m_uniqueIdOfEnemyAll;			// 敵全体の中での識別ID
	Common::ENEMY_KIND	m_enemyKind;					// 敵の種類
	uint32_t			m_HP;							// 敵体力
	math::Vector2		m_eye;							// 敵の視線
	Game2DBase			*m_pEnemy2D;					// 敵の画像等管理
	TEX_DRAW_INFO		m_enemyInfo;					// 描画関係情報

	Texture2D			m_textureEnemy;					// 敵画像
	Texture2D			m_textureLife;					// 敵ライフ画像

	Common::ENEMY_AI	m_nextAI;						// 次に思考するAIステート
};

#endif