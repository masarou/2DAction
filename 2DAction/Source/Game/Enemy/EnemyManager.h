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

#include "System/Task/SystemTaskUnit.h"
#include "System/SystemDefine.h"
#include "EnemyBase.h"

class GamePlayer;
class Bullet;

class EnemyManager : TaskUnit
{

public:

	static EnemyManager *CreateEnemyManager();

	void CreateEnemy( const Common::ENEMY_KIND &kind );		// 敵キャラ生成
	void DeleteEnemy( const uint32_t &uniqueNumber );		// 指定クラスを管理から外す

	bool CheckCollision( const TEX_DRAW_INFO &texInfo );	// 何かしらの描画物が敵に当たっているかどうかチェック
	bool CheckCollisionToBullet( Bullet *bullet );			// プレイヤーの弾との当たり判定
	bool CheckCollisionToPlayer( GamePlayer *player );		// プレイヤーとの当たり判定

protected:
	
	virtual void Update() override;
	virtual void CollisionUpdate() override;
	virtual void DrawUpdate() override;

	// メッセージ処理
	void EventUpdate( const Common::CMN_EVENT &eventId ) override;

private:

	EnemyManager(void);
	~EnemyManager(void);
	
	std::vector<EnemyBase*> m_enemyArray;	// 敵管理

};

#endif

