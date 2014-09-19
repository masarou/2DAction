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

class EnemyBase : public TaskUnit
{
private:


protected:

	EnemyBase( std::string jsonName, uint32_t uniqueId, Common::ENEMY_KIND kind );
	virtual ~EnemyBase(void);

	virtual void DrawUpdate() override;
	virtual bool DieMain() override;

	// 他クラスからのイベント処理
	void EventUpdate( const Common::CMN_EVENT &eventId ) override;
	void HitPlayreBullet();

public:

	uint32_t			m_uniqueIdOfEnemyAll;	// 敵全体の中での識別ID
	Common::ENEMY_KIND	m_enemyKind;			// 敵の種類

	uint32_t			m_HP;					// 敵体力
	math::Vector2		m_eye;					// 敵の視線
	Game2DBase			*m_enemy2D;				// 敵の画像等管理
	TEX_DRAW_INFO		m_enemyInfo;			// 描画関係情報
};

#endif