/* ====================================================================== */
/**
 * @brief  敵AIのBaseクラス
 *
 * @note
 *		
 */
/* ====================================================================== */

#ifndef ENEMY_AI_BASE
#define ENEMY_AI_BASE

#include "System/Draw2D/SystemDraw2DDefine.h"

class EnemyBase;

class EnemyAIBase
{

public:

	//static EnemyAIBase *Create( EnemyBase *enemyMine = NULL );
	virtual ~EnemyAIBase(void);

	void SetThingingEnemy( EnemyBase *enemyMine );
	const bool &IsReady(){ return m_isReady; }	// 初期化準備完了かどうか
	void Exec( TEX_DRAW_INFO &enemyInfo );		// 思考開始

protected:

	virtual bool InitAI(){ return true; }	// AI初期化
	virtual void ExecMain( TEX_DRAW_INFO &enemyInfo ){};	// 派生先でのAI実装

private:
	
	EnemyAIBase();
	EnemyAIBase( EnemyBase *enemyMine );
	
	// 現在の自分の状態を知るために保持しておく
	EnemyBase	*m_enemyMine;
	bool		m_isReady;		// 思考準備が終了したかどうか
};
#endif