/* ====================================================================== */
/**
 * @brief  敵WizardクラスAI
 *
 * @note
 *		デフォルトAI
 */
/* ====================================================================== */

#ifndef __ENEMY_AI_WIZARD__
#define __ENEMY_AI_WIZARD__

#include "EnemyAIBase.h"
#include "Game/Enemy/EnemyWizard.h"

class AIWizard : public EnemyAIBase
{
public:

	// 次の各行動に移るまでの待ち時間
	static const uint32_t ACTION_INTERVAL_SHORT = 60;
	static const uint32_t ACTION_INTERVAL = 90;

	static AIWizard *Create();

	// AIの種類を派生先でセットしておく
	virtual const Common::ENEMY_AI GetAIKind() const{ return Common::AI_ATTACK_WIZARD; }

protected:
	
	virtual bool InitAI() override;								// AI初期化
	virtual void ExecMain( TEX_DRAW_INFO &enemyInfo, ACTION_ARRAY &actionInfo ) override;	// 派生先でのAI実装
	virtual void EnemyRecievedEvent( const Common::CMN_EVENT &eventInfo ) override;			// 敵クラスが他のクラスからもらったイベント

private:

	enum ACTION_TYPE{
		ACTION_NORMAL,				// 単発の弾を撃つ
		ACTION_CONTINUE_BULLET,		// 複数の弾を連続して撃つ
		ACTION_WAY_BULLET,			// 周りに一定数を弾を一度に放つ
		ACTION_SET_CRYSTAL_PLAYER,	// 相手プレイヤーにクリスタルを放つ

		ACTION_WORP,				// ワープ移動

		ACTION_MAX,
	};

	ACTION_TYPE GetRandamNextAction();
	void ChangeActionType( const ACTION_TYPE &nextAction );
	void SetInterruptAction( const ACTION_TYPE &interruptAction );
	bool ActionNormal();
	bool ActionSetCrystalPlayer();
	bool ActionWayBullet();

	// コルーチンを使用している関数の宣言、定義は同じファイルに書かないとエラーが出る模様
	bool ActionContinueBullet(){
		const EnemyWizard *const pWizard = static_cast<const EnemyWizard*const>( GetEnemeyMine() );
		reenter( m_coro ){
			for( uint32_t i = 0; i < pWizard->GetCrystalIndex() ; ++i ){
				ShootBullet( pWizard->GetCrystalPos( i ), math::Vector2(), pWizard->GetBulletDamage() );
			}
			m_waitCounter = 5;
			yield return false;
			
			for( uint32_t i = 0; i < pWizard->GetCrystalIndex() ; ++i ){
				ShootBullet( pWizard->GetCrystalPos( i ), math::Vector2(), pWizard->GetBulletDamage() );
			}
			m_waitCounter = 5;
			yield return false;
			
			for( uint32_t i = 0; i < pWizard->GetCrystalIndex() ; ++i ){
				ShootBullet( pWizard->GetCrystalPos( i ), math::Vector2(), pWizard->GetBulletDamage() );
			}
			m_waitCounter = 5;
			yield return false;
			
			for( uint32_t i = 0; i < pWizard->GetCrystalIndex() ; ++i ){
				ShootBullet( pWizard->GetCrystalPos( i ), math::Vector2(), pWizard->GetBulletDamage() );
			}
			m_waitCounter = 5;
			yield return false;
			
			for( uint32_t i = 0; i < pWizard->GetCrystalIndex() ; ++i ){
				ShootBullet( pWizard->GetCrystalPos( i ), math::Vector2(), pWizard->GetBulletDamage() );
			}
			m_waitCounter = ACTION_INTERVAL_SHORT;
		}
		
		ChangeActionType( GetRandamNextAction() );

		return true;
	}

	bool ActionEnemyWorp( TEX_DRAW_INFO &enemyInfo ){
		reenter( m_coro ){
			
			m_waitCounter = Utility::GetRandamValue( 20, 10 );
			yield return false;

			// ランダムな位置をセット
			enemyInfo.m_posOrigin = Utility::GetMapRandamPos( /*bool allowInWindow=*/true );

			// 一定時間後移動先で待機
			m_waitCounter = 20;
			GameEffect::CreateEffect( GameEffect::EFFECT_WORP, enemyInfo.m_posOrigin );
			yield return false;

			// 次のアクションセット
			ChangeActionType( GetRandamNextAction() );
		}
		return true;
	}

	AIWizard(void);
	~AIWizard(void);

	ACTION_TYPE		m_currAction;
	ACTION_TYPE		m_nextAction;		// 次にセットされた行動
	math::Vector2	m_nextWorpPos;	// ワープ時の移動先
	uint32_t		m_waitCounter;	// 連続で同じ行動をしないためのカウンタ
	coroutine		m_coro;			// コルーチン
};


#endif