/* ====================================================================== */
/**
 * @brief  敵WizardクラスAI
 *
 * @note
 *		デフォルトAI
 */
/* ====================================================================== */

#ifndef __ENEMY_AI_DRAGON__
#define __ENEMY_AI_DRAGON__

#include "EnemyAIBase.h"
#include "Game/Enemy/EnemyWizard.h"
#include "System/Sound/SystemSoundManager.h"

class DragonFireBomb;

class AIDragon : public EnemyAIBase
{
public:

	// 次の各行動に移るまでの待ち時間
	static const uint32_t ACTION_INTERVAL_SHORT = 30;
	static const uint32_t ACTION_INTERVAL = 60;
	static const uint32_t ACTION_INTERVAL_LONG = 100;

	static AIDragon *Create();

	// AIの種類を派生先でセットしておく
	virtual const Common::ENEMY_AI GetAIKind() const{ return Common::AI_ATTACK_DRAGON; }

protected:
	
	virtual bool InitAI() override;								// AI初期化
	virtual void ExecMain( TEX_DRAW_INFO &enemyInfo, ACTION_ARRAY &actionInfo ) override;	// 派生先でのAI実装
	virtual void EnemyRecievedEvent( const Common::CMN_EVENT &eventInfo ) override;			// 敵クラスが他のクラスからもらったイベント

private:

	enum ACTION_TYPE{
		ACTION_MOVE_AWAY,		// 逃げるような移動
		ACTION_FIRE_BOMB,		// 炎の弾発射
		ACTION_LARGE_EXPLOSION,	// 大爆発

		ACTION_MAX,
	};

	ACTION_TYPE GetRandamNextAction();
	void ChangeActionType( const ACTION_TYPE &nextAction );
	void SetInterruptAction( const ACTION_TYPE &interruptAction );

	// 相手から逃げる時の目的地セット
	bool SetRunAwayPosFromPlayer( TEX_DRAW_INFO &enemyInfo );

	// 設定した目的地まで移動(trueが帰ってくれば終了)
	bool DashMove( TEX_DRAW_INFO &enemyInfo, const math::Vector2 &targetPos );

	// 各行動に対応した関数
	// コルーチンを使用している関数の宣言、定義は同じファイルに書かないとエラーが出る模様
	bool ActionShootFireBall( TEX_DRAW_INFO &enemyInfo, const bool &onlyAction = false );
	bool ActionFireWall( TEX_DRAW_INFO &enemyInfo ){

		static uint32_t fireCounter = 0;

		reenter( m_coro ){
			// 炎上前エフェクト
			GameEffect::CreateEffect( GameEffect::EFFECT_PRE_FIRE_WALL, enemyInfo.m_posOrigin );
			m_waitCounter = ACTION_INTERVAL_LONG;
			yield return false;
			
			// 炎上エフェクト
			GameEffect::CreateEffect( GameEffect::EFFECT_FIRE_WALL, WINDOW_CENTER );
			SoundManager::GetInstance()->PlaySE("PreExplosion");

			// 炎生成(レベル数分)
			for( fireCounter = 0 ; fireCounter < GetEnemyLevel() ; ++fireCounter ){
				GameEffectWithCollision::CreateEffect( Common::OWNER_ENEMY, GameEffectWithCollision::EFFECT_FIRE, Utility::GetMapRandamPos( /*bool allowInWindow=*/true ) );
				m_waitCounter = 5;
				yield return false;
			}
		}

		fireCounter = 0;

		// 次の行動セット
		if( math::IsInRange( Utility::GetPlayerPos(), enemyInfo.m_posOrigin, 400.0f ) )
		{
			// 近くにプレイヤーがいるなら逃げる
			ChangeActionType( ACTION_MOVE_AWAY );
		}
		else{
			ChangeActionType( GetRandamNextAction() );
		}
		
		m_waitCounter = ACTION_INTERVAL_LONG;
		return true;
	}

	bool ActionAwayFromPlayer( TEX_DRAW_INFO &enemyInfo ){

		reenter( m_coro ){

			// 移動先セット
			SetRunAwayPosFromPlayer( enemyInfo );
			yield return false;

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
		ChangeActionType( GetRandamNextAction() );
		m_waitCounter = ACTION_INTERVAL_LONG;
		return true;
	}



	AIDragon(void);
	~AIDragon(void);

	ACTION_TYPE		m_currAction;
	ACTION_TYPE		m_nextAction;	// 次にセットされた行動
	math::Vector2	m_movingPos;	// 移動するときの目的地
	uint32_t		m_waitCounter;	// 連続で同じ行動をしないためのカウンタ

	coroutine		m_coro;			// コルーチン
};


class DragonFireBomb : public TaskUnit, public Collision2DUnit
{
public:

	static DragonFireBomb *Create( const math::Vector2 &pos, const math::Vector2 &vec, const uint32_t &damage );

protected:


	virtual bool Init() override;
	virtual bool DieMain() override;
	virtual void Update() override;
	virtual void DrawUpdate() override;

	// ほかのクラスからのイベント処理
	virtual void EventUpdate( Common::CMN_EVENT &eventId ) override;

	virtual const Common::TYPE_OBJECT GetTypeObject() const;

private:

	DragonFireBomb( const math::Vector2 &pos, const math::Vector2 &vec, const uint32_t &damage );
	~DragonFireBomb(void);
	
	uint32_t	m_liveTime;		// 生成してからの時間
	uint32_t	m_liveTimeMax;	// 生成後の最大生存時間

	math::Vector2	m_vec;
	uint32_t		m_damage;

};

#endif