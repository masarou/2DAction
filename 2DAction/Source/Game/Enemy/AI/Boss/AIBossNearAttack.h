/* ====================================================================== */
/**
 * @brief  敵AIの通常クラス(プレイヤー未発見)
 *
 * @note
 *		デフォルトAI
 */
/* ====================================================================== */

#ifndef __AI_BOSS_NEAE_ATTACK__
#define __AI_BOSS_NEAE_ATTACK__

#include "Game/Enemy/AI/EnemyAIBase.h"
#include "Game/Enemy/EnemyBoss.h"
#include "Common/Utility/CommonGameUtility.h"

class AIBossNearAttack : public EnemyAIBase
{
public:

	static AIBossNearAttack *Create();

	// AIの種類を派生先でセットしておく
	virtual const Common::ENEMY_AI GetAIKind() const{ return Common::AI_ATTACK_NEAR; }

protected:
	
	virtual bool InitAI() override;								// AI初期化
	virtual void ExecMain( TEX_DRAW_INFO &enemyInfo, ACTION_ARRAY &actionInfo ) override;	// 派生先でのAI実装

private:

	enum ACTION_TYPE{
		ACTION_SPREAD_BULLET,
		ACTION_SLASHING,
		ACTION_MOVE,

		ACTION_MAX,
	};

	void ChangeActionType( const ACTION_TYPE &nextType );

	// 次の行動を取得
	ACTION_TYPE GetNextAction();
	
	// コルーチンで呼び続けることで目的地までの移動を行う trueで目的地に到達
	bool DashMove( TEX_DRAW_INFO &enemyInfo, const math::Vector2 &targetPos );

	// ショットガン的な攻撃
	bool ExecSpreadBullet( TEX_DRAW_INFO &enemyInfo );

	// 移動的な攻撃
	bool ExecMoving( TEX_DRAW_INFO &enemyInfo )
	{
		reenter( m_coro ){

			// ランダムな位置を目的地にする
			m_movingPos = Utility::GetMapRandamPos( /*bool allowInWindow=*/true );

			// 移動
			for(;;){
				if( DashMove( enemyInfo, m_movingPos ) ){
					break;
				}
				if( GetNowCount()%5 == 0 ){
					ShootBullet( enemyInfo.m_posOrigin, math::Vector2(), 45, 12 );
				}
				yield return false;
			}
		}
		// 目的地初期化
		m_movingPos = math::Vector2();

		// 次の行動セット
		ChangeActionType( GetNextAction() );
		m_actionIntervalTime = Utility::GetRandamValue( 60, 40 );
		return true;
	}

	// プレイヤーから距離を取る時の目的地設定
	bool SetRunAwayPosFromPlayer( TEX_DRAW_INFO &enemyInfo );

	// 斬撃攻撃
	bool ExecSlashing( TEX_DRAW_INFO &enemyInfo )
	{
		uint32_t currTime = GetNowCount();
		const EnemyBoss *const pBoss = static_cast<const EnemyBoss*const>( GetEnemeyMine() );

		reenter( m_coro ){

			// プレイヤーに急接近
			for(;;){
				if( DashMove( enemyInfo, Utility::GetPlayerPos() ) ){
					break;
				}
				yield return false;
			}

			// 斬撃攻撃
			Slashing( Slashing::TYPE_3RD, pBoss->GetSlashingDamage() );
			yield return false;

			// 距離を取る目標となる位置設定
			SetRunAwayPosFromPlayer( enemyInfo );

			// 距離を取る
			for(;;){
				if( DashMove( enemyInfo, m_movingPos ) ){
					break;
				}
				yield return false;
			}
		}
		// 目的地初期化
		m_movingPos = math::Vector2();
		
		// 次の行動セット
		ChangeActionType( GetNextAction() );
		m_actionIntervalTime = Utility::GetRandamValue( 60, 40 );
		return true;
	}

	AIBossNearAttack(void);
	~AIBossNearAttack(void);

	coroutine		m_coro;					// コルーチン
	uint32_t		m_actionIntervalTime;	// アクションを起こしてから次のアクションまでの余暇時間

	ACTION_TYPE		m_currActionType;		// 現在のアクション
	ACTION_TYPE		m_nextActionType;		// 次に設定されているアクション

	// 主に斬撃で使用するメンバ
	uint32_t		m_currActionTime;		// 現在のアクションタイプになってからの時間

	// 斬撃等の移動時に使用する目的地
	math::Vector2	m_movingPos;
};

#endif