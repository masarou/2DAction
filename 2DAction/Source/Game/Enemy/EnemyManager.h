/* ====================================================================== */
/**
 * @brief  敵管理クラス
 *
 * @note
 *		
 */
/* ====================================================================== */

#include "System/Task/SystemTaskUnit.h"
#include "System/SystemDefine.h"
#include "EnemyBase.h"

class Bullet;

class EnemyManager : TaskUnit
{

public:

	static EnemyManager *GetInstance();
	void CreateEnemy( Common::ENEMY_KIND kind );	// 敵キャラ生成
	void DeleteEnemy( uint32_t uniqueNumber );		// 指定クラスを管理から外す
	void CheckCollision( Bullet *bullet );			// プレイヤーの弾との当たり判定

protected:
	
	virtual void Update() override;
	virtual void DrawUpdate() override;

	// メッセージ処理
	void EventUpdate( const Common::CMN_EVENT &eventId ) override;

private:

	EnemyManager(void);
	~EnemyManager(void);
	
	static EnemyManager		*m_pInstance;
	std::vector<EnemyBase*> m_enemyArray;	// 敵管理

};

