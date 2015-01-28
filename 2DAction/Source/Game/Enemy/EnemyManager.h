/* ====================================================================== */
/**
 * @brief  敵管理クラス
 *
 * @note
 *		
 */
/* ====================================================================== */

#ifndef ENEMY_MANAGER
#define ENEMY_MANAGER

#include "System/SystemDefine.h"
#include "System/Task/SystemTaskUnit.h"
#include "System/Message/SystemMessageUnit.h"
#include "System/Draw2D/SystemDraw2DDefine.h"

class GamePlayer;
class Bullet;
class EnemyBase;

class EnemyManager : public TaskUnit
{

public:

	static EnemyManager *CreateEnemyManager();

	void AddEnemy( const Common::ENEMY_KIND &kind );				// 敵キャラ生成
	void RemoveEnemy( EnemyBase *removeEnemy );						// 指定クラスを管理から外す

	// 指定の敵の数を数える
	uint32_t CountEnemy( const Common::ENEMY_KIND &kind = Common::ENEMY_KIND_MAX ) const;

protected:
	
	virtual bool DieMain() override;

private:

	EnemyManager(void);
	~EnemyManager(void);

	std::vector<EnemyBase*> m_enemyArray;	// 敵管理
};

#endif

