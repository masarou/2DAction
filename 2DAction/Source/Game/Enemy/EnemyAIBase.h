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

#include "EnemyBase.h"
#include "Common/CommonDefine.h"
#include "System/Draw2D/SystemDraw2DDefine.h"

class EnemyBase;

class EnemyAIBase
{

public:

	virtual ~EnemyAIBase(void);

	void SetThingingEnemy( EnemyBase *enemyMine );
	const bool &IsReady(){ return m_isReady; }	// 初期化準備完了かどうか
	void Exec( TEX_DRAW_INFO &enemyInfo );		// 思考開始

	// AIの種類を派生先でセットしておく
	virtual const Common::ENEMY_AI GetAIKind() const = 0 ;

	// 敵キャラそのものに関するget,set関数
	void SetEnemyAnim( const std::string &animTag );
	std::string GetEnemyAnim();
	void SetEnemyEyeSight( math::Vector2 &eye );
	const math::Vector2 &GetEnemyEyeSight() const;
	void ChangeEnemyAI( Common::ENEMY_AI nextAI );

protected:
	
	EnemyAIBase();

	virtual bool InitAI(){ return true; }					// AI初期化
	virtual void ExecMain( TEX_DRAW_INFO &enemyInfo ){};	// 派生先でのAI実装
		
private:
	
	EnemyAIBase( EnemyBase *enemyMine );
	bool		m_isReady;		// 思考準備が終了したかどうか
	EnemyBase	*m_enemyMine;	// 現在の自分の状態を知るために保持しておく
};
#endif