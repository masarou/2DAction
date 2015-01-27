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

	uint32_t CountEnemy() const{ return m_enemyArray.size(); }

protected:
	
	virtual void CollisionUpdate() override;
	virtual bool DieMain() override;

private:

	EnemyManager(void);
	~EnemyManager(void);

	bool CheckCollision( const TEX_DRAW_INFO &texInfo ) const;		// 何かしらの描画物が敵に当たっているかどうかチェック
	//bool CheckCollisionToBullet( const Bullet *bullet );			// プレイヤーの弾との当たり判定
	//bool CheckCollisionToPlayer( const GamePlayer *player ) const;	// プレイヤーとの当たり判定

	std::vector<EnemyBase*> m_enemyArray;	// 敵管理
};

#endif

