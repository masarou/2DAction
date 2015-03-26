/* ====================================================================== */
/**
 * @brief  敵AIのBaseクラス
 *
 * @note
 *		
 */
/* ====================================================================== */

#ifndef __ENEMY_AI_BASE__
#define __ENEMY_AI_BASE__

#include "Game/Enemy/AI/EnemyAIBase.h"
#include "Common/CommonDefine.h"
#include "System/Draw2D/SystemDraw2DDefine.h"

#include "EnemyAIDefine.h"

class EnemyBase;

// 共通での距離の認識
static const uint32_t DISTANCE_TO_PLAYER_NEAR	= 300;
static const uint32_t DISTANCE_TO_PLAYER_MIDDLE	= 600;
static const uint32_t DISTANCE_TO_PLAYER_FAR	= 1000;

class EnemyAIBase
{

public:

	virtual ~EnemyAIBase(void);

	void SetThingingEnemy( EnemyBase *enemyMine );
	const bool &IsReady(){ return m_isReady; }	// 初期化準備完了かどうか
	void Exec( TEX_DRAW_INFO &enemyInfo, ACTION_ARRAY &actionInfo );		// 思考開始

	// AIの種類を派生先でセットしておく
	virtual const Common::ENEMY_AI GetAIKind() const = 0 ;

	// 敵キャラそのものに関するget,set関数
	void SetEnemyAnim( const std::string &animTag );
	std::string GetEnemyAnim();
	void SetEnemyEyeSight( math::Vector2 &eye );
	math::Vector2 GetEnemyEyeSight() const;
	void ChangeEnemyAI( Common::ENEMY_AI nextAI );
	Common::ENEMY_KIND GetEnemyKind() const;
	const std::string &GetEnemyJsonName() const;

protected:
	
	EnemyAIBase();

	virtual bool InitAI(){ return true; }					// AI初期化
	virtual void ExecMain( TEX_DRAW_INFO &enemyInfo, ACTION_ARRAY &actionInfo ) = 0;	// 派生先でのAI実装
		
private:
	
	EnemyAIBase( EnemyBase *enemyMine );
	bool		m_isReady;		// 思考準備が終了したかどうか
	EnemyBase	*m_enemyMine;	// 現在の自分の状態を知るために保持しておく
};
#endif