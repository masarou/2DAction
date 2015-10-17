/* ====================================================================== */
/**
 * @brief  敵管理クラス
 *
 * @note
 *		敵の生成は必ずGameManager(friendクラス)を通して行われます
 */
/* ====================================================================== */

#ifndef __ENEMY_MANAGER__
#define __ENEMY_MANAGER__

#include "System/SystemDefine.h"
#include "System/Task/SystemTaskUnit.h"
#include "System/Message/SystemMessageUnit.h"
#include "System/Draw2D/SystemDraw2DDefine.h"
#include "Game/GameManager.h"

class GamePlayer;
class Bullet;
class EnemyBase;

class EnemyManager : public TaskUnit
{
	friend class GameManager;

public:

	static EnemyManager *CreateEnemyManager();

	void RemoveEnemy( EnemyBase *removeEnemy );				// 指定クラスを管理から外す

	// 指定の敵の数を数える
	uint32_t CountEnemy( const Common::ENEMY_KIND &kind = Common::ENEMY_KIND_MAX ) const;

	// AI(AI_MOVE_PLAYER)の敵の数を数える
	uint32_t CountMovePlayerAI() const{ return m_moveToPlayerNum; }

	// SystemMessageUnitのユニークIDから敵クラスを取得
	const EnemyBase *GetEnemyBymessageUniqueID( uint32_t messageUnitID );

	// 指定位置からDistance以内に敵がいるかどうか取得
	bool IsNearEnemy( const math::Vector2 &checkPos, float distance );

protected:
	
	virtual void Update() override;
	virtual bool DieMain() override;

private:

	void AddEnemy( const Common::ENEMY_KIND &kind, const uint32_t &enemyLevel, const math::Vector2 &enemyPos );		// 敵キャラ生成

	EnemyManager(void);
	~EnemyManager(void);

	uint32_t	m_moveToPlayerNum;
	std::vector<EnemyBase*> m_enemyArray;	// 敵管理
};

#endif

