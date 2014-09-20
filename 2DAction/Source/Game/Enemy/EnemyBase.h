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

#include "System/SystemDefine.h"
#include "Game/Game2DBase.h"

class EnemyBase
{

public:

	virtual ~EnemyBase(void);
	uint32_t GetUniqueNumber(){ return m_uniqueIdOfEnemyAll; }

	// 他クラスからのイベント処理
	void EventUpdate( const Common::CMN_EVENT &eventId );

protected:

	EnemyBase( std::string jsonName, uint32_t uniqueId, Common::ENEMY_KIND kind );

	virtual void UpdateEnemy(){};	// 位置やAIによる数値周りの更新
	virtual void DrawEnemy();		// 描画更新
	void HitPlayreBullet();			// 弾が当たった時の処理

protected:

	uint32_t			m_uniqueIdOfEnemyAll;	// 敵全体の中での識別ID
	Common::ENEMY_KIND	m_enemyKind;			// 敵の種類

	uint32_t			m_HP;					// 敵体力
	math::Vector2		m_eye;					// 敵の視線
	Game2DBase			*m_enemy2D;				// 敵の画像等管理
	TEX_DRAW_INFO		m_enemyInfo;			// 描画関係情報
};

#endif