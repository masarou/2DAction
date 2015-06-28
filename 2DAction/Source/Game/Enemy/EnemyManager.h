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

protected:
	
	virtual bool DieMain() override;

private:

	void AddEnemy( const Common::ENEMY_KIND &kind, const uint32_t &enemyLevel );		// 敵キャラ生成

	EnemyManager(void);
	~EnemyManager(void);

	std::vector<EnemyBase*> m_enemyArray;	// 敵管理
};

#endif

