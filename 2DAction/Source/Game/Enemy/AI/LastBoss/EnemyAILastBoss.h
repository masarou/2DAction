/* ====================================================================== */
/**
 * @brief  ラスボスAI
 *
 * @note
 *		
 */
/* ====================================================================== */

#ifndef __ENEMY_AI_LAST_BOSS__
#define __ENEMY_AI_LAST_BOSS__

#include "Game/GameRegister.h"
#include "Game/Enemy/AI/EnemyAIBase.h"
#include "Common/Utility/CommonGameUtility.h"
#include "System/Sound/SystemSoundManager.h"

class LastBossRight;
class LastBossLeft;

class EnemyAILastBoss : public EnemyAIBase
{
public:

	static EnemyAILastBoss *Create();

	// AIの種類を派生先でセットしておく
	virtual const Common::ENEMY_AI GetAIKind() const{ return Common::AI_ATTACK_LAST_BOSS; }

protected:
	
	virtual bool InitAI() override;								// AI初期化
	virtual void ExecMain( TEX_DRAW_INFO &enemyInfo, ACTION_ARRAY &actionInfo ) override;	// 派生先でのAI実装
	virtual void EnemyIsDead();

private:

	void ExecHandsUpdate( TEX_DRAW_INFO &enemyInfo );

	EnemyAILastBoss(void);
	virtual ~EnemyAILastBoss(void);

	// 操作する両手クラス
	LastBossRight	*m_pRightHand;
	LastBossLeft	*m_pLeftHand;

	// ふわふわ浮くので、描画とは別に基準となる位置を持つ
	math::Vector2 m_moveBasicPos;

};


/* ====================================================================== */
/**
 * @brief  ラスボス右手左手クラス
 *
 * @note
 *		
 */
/* ====================================================================== */
class LastBossHand : public TaskUnit, public Collision2DUnit
{
public:

	enum ACTION_KIND{
		ACTION_NONE,	// 何もしない(定位置へ)
		ACTION_FIST,	// パンチ
		ACTION_GUARD,	// 本体をガード
		ACTION_SUMMON,	// モンスター召喚(ユニークキャラ)
		ACTION_SUMMONS,	// モンスター召喚(雑魚キャラ)
		ACTION_MAX,
	};

	void SetBasicPos( math::Vector2 basicPos );

	// 指定されているアクション実行(終了したらtrue)
	bool ExecAction();

	void SetAnotherHand( LastBossHand *pHand );

	// 現在の実行アクションを取得
	const ACTION_KIND &GetCurrentActionKind() const{ return m_currAction; }

	// 死亡時にボスAIから呼ばれる
	void StartDie(){ TaskStartDie(); }

protected:

	LastBossHand( const std::string &readFileName, const math::Vector2 &enemyPos );
	virtual ~LastBossHand(void);

	virtual bool Init() override;
	virtual void Update() override;			// 移動等の内部数値の更新
	virtual void DrawUpdate() override;		// 描画更新
	virtual bool DieMain() override{return true;}

	// 行動するときの回転量を取得
	virtual float GetRotateDefault() = 0;
	virtual float GetRotActionFist() = 0;
	virtual float GetRotActionSummon() = 0;

	// 各種アクション内での右手、左手用位置調整オフセット
	virtual math::Vector2 GetSummonEffectOffset() = 0;
	
	// ユニークモンスターを生成した回数(両手で共通)
	static uint32_t	m_createUniqueEnemyCount;

private:

	// パンチアクション
	virtual bool ExecFist( TEX_DRAW_INFO &drawInfo )
	{
		math::Vector2 currentOffset = drawInfo.m_posOrigin - m_basicPos;

		reenter( m_coro ){

			// 腕の振り上げ
			for(;;){
				if( MoveToTargetPos( 
					m_basicPos + math::Vector2( -100.0f, -150.0f )
					, 15.0f
					, 0.1f )
					){
					m_waitCounter = 30;
					break;
				}
				yield return false;
			}
			
			// 振りおろし先を決める
			m_moveTargetPos = Utility::GetPlayerPos();
			m_waitCounter = 10;
			yield return false;

			// 目的地まで移動
			for(;;){
				if( MoveToTargetPos( m_moveTargetPos, 20.0f, 0.4f ) ){
					m_waitCounter = 30;
					break;
				}
				yield return false;
			}

			// 元の位置に戻る
			for(;;){
				if( MoveToTargetPos( m_basicPos, 10.0f, 0.3f ) ){
					m_waitCounter = 120;
					break;
				}
				yield return false;
			}

			drawInfo.m_rot = GetRotateDefault();
			m_moveTargetPos = math::Vector2();
		}
		
		return true;
	}

	
	// モンスター召喚(ユニーク)
	virtual bool ExecSummonMonster( TEX_DRAW_INFO &drawInfo )
	{
		reenter( m_coro ){
			// ランダムな位置に移動
			for(;;){
				m_moveTargetPos = Utility::GetMapRandamPos( /*bool allowInWindow =*/true );
				// 反対の手がある位置に行くと不自然なので今いる位置側に限定
				if( ( GetTypeObject() == Common::TYPE_LAST_BOSS_LEFT
					&& m_moveTargetPos.x < drawInfo.m_posOrigin.x )
					|| ( GetTypeObject() == Common::TYPE_LAST_BOSS_RIGHT
					&& m_moveTargetPos.x > drawInfo.m_posOrigin.x ) )
				{
					// 移動できる場所かチェック
					if( Utility::GetMapHeight( m_moveTargetPos + GetSummonEffectOffset() ) == 0){
						break;
					}
				}
			}
			for(;;){
				if( MoveToTargetPos( m_moveTargetPos, 5.0f, 0.3f ) ){
					m_waitCounter = 30;
					RotateToTargetAngle( GetRotActionSummon(), /*bool isForceSet=*/true );
					break;
				}
				else if( drawInfo.m_rot != GetRotActionSummon() ){
					// 移動しつつ自然な角度に回転させる
					RotateToTargetAngle( GetRotActionSummon() );
				}
				yield return false;
			}

			// 召喚エフェクト
			GameEffect::CreateEffect( GameEffect::EFFECT_SUMMON, drawInfo.m_posOrigin + GetSummonEffectOffset() );
			m_waitCounter = 20;
			yield return false;

			// モンスター召喚
			GameRegister::GetInstance()->UpdateManagerGame()->CreateEnemy( DecideCreateMonster(), 0, true, drawInfo.m_posOrigin + GetSummonEffectOffset() );
			SoundManager::GetInstance()->PlaySE("Summon");

			// 元の位置に戻る
			for(;;){
				if( MoveToTargetPos( m_basicPos, 10.0f, 0.3f ) ){
					m_waitCounter = 30;
					RotateToTargetAngle( GetRotateDefault(), /*bool isForceSet=*/true );
					break;
				}
				else if( drawInfo.m_rot != GetRotateDefault() ){
					// 移動しつつ自然な角度に回転させる
					RotateToTargetAngle( GetRotateDefault() );
				}
				yield return false;
			}
		}
		m_waitCounter = 120;
		return true;
	}

	
	// 雑魚モンスター召喚
	bool ExecSummonLightMonsters( TEX_DRAW_INFO &drawInfo )
	{
		reenter( m_coro ){

			// 一定数モンスターを生成
			for(;;){

				// ランダムな位置に移動
				for(;;){
					m_moveTargetPos = Utility::GetMapRandamPos( /*bool allowInWindow =*/true );
					// 反対の手がある位置に行くと不自然なので今いる位置側に限定
					if( ( GetTypeObject() == Common::TYPE_LAST_BOSS_LEFT
						&& m_moveTargetPos.x < drawInfo.m_posOrigin.x )
						|| ( GetTypeObject() == Common::TYPE_LAST_BOSS_RIGHT
						&& m_moveTargetPos.x > drawInfo.m_posOrigin.x ) )
					{
						// 移動できる場所かチェック
						if( Utility::GetMapHeight( m_moveTargetPos + GetSummonEffectOffset() ) == 0){
							break;
						}
					}
				}
				for(;;){
					if( MoveToTargetPos( m_moveTargetPos, 5.0f, 0.3f ) ){
						m_waitCounter = 30;
						RotateToTargetAngle( GetRotActionSummon(), /*bool isForceSet=*/true );
						break;
					}
					else if( drawInfo.m_rot != GetRotActionSummon() ){
						// 移動しつつ自然な角度に回転させる
						RotateToTargetAngle( GetRotActionSummon() );
					}
					yield return false;
				}

				// 召喚エフェクト
				GameEffect::CreateEffect( GameEffect::EFFECT_SUMMON, drawInfo.m_posOrigin + GetSummonEffectOffset() );
				m_waitCounter = 20;
				yield return false;

				// モンスター召喚
				GameRegister::GetInstance()->UpdateManagerGame()->CreateEnemy( DecideCreateLightMonster(), 0, true, drawInfo.m_posOrigin + GetSummonEffectOffset() );
				SoundManager::GetInstance()->PlaySE("Summon");

				// モンスター召喚続行かどうか
				if( !IsCreateLightMonster() ){
					// モンスター生成終了
					// 元の位置に戻る
					for(;;){
						if( MoveToTargetPos( m_basicPos, 10.0f, 0.3f ) ){
							m_waitCounter = 30;
							RotateToTargetAngle( GetRotateDefault(), /*bool isForceSet=*/true );
							break;
						}
						else if( drawInfo.m_rot != GetRotateDefault() ){
							// 移動しつつ自然な角度に回転させる
							RotateToTargetAngle( GetRotateDefault() );
						}
						yield return false;
					}
					break;
				}
				else{
					// 引き続きモンスター生成
				}
			}
		}
		m_waitCounter = 120;
		return true;
	}


	// 次のアクションを求める
	ACTION_KIND GetNextActionKind();

	// モンスターを作成してもよいかどうかチェック
	uint32_t CountUniqueMonster() const;
	bool IsCreateUniqueMonster() const;
	bool IsCreateLightMonster() const;

	// 次に生成するモンスターを決定
	Common::ENEMY_KIND DecideCreateMonster() const;
	Common::ENEMY_KIND DecideCreateLightMonster() const;

	// 指定先に移動 trueで到達
	bool MoveToTargetPos( const math::Vector2 &targetPos, const float &maxSpeed, const float &rateSpeed );

	// 指定の回転角度まで一定量で回転
	bool RotateToTargetAngle( const math::Angle &targetDegree, const bool &forceSet = false );

	// 基準点となる位置情報
	math::Vector2	m_basicPos;

	// 移動している時の目標地点
	math::Vector2	m_moveTargetPos;

	// 次の行動に移すまでの時間
	uint32_t		m_waitCounter;

	// 保持している実行アクションの種類
	ACTION_KIND		m_currAction;
	ACTION_KIND		m_nextAction;

	// もう片方の腕のポインタ
	LastBossHand	*m_pAnotherHand;
	
	// コルーチン
	coroutine		m_coro;

};

class LastBossRight : public LastBossHand
{
public:
	
	static LastBossRight *Create( const std::string &readFileName, const math::Vector2 &enemyPos );

protected:
	
	LastBossRight( const std::string &readFileName, const math::Vector2 &enemyPos );

	// クラスの敵タイプ
	virtual const Common::TYPE_OBJECT GetTypeObject() const override{ return Common::TYPE_LAST_BOSS_LEFT; }

	// 行動するときの回転量を取得
	virtual float GetRotateDefault() override{ return 20.0f; }
	virtual float GetRotActionFist() override{ return 0.0f; }
	virtual float GetRotActionSummon() override{ return 90.0f; }
	
	virtual math::Vector2 GetSummonEffectOffset() override{ return math::Vector2( -60.0f, 70.0f ); }

private:

	virtual ~LastBossRight(void);

};

class LastBossLeft : public LastBossHand
{
public:

	static LastBossLeft *Create( const std::string &readFileName, const math::Vector2 &enemyPos );

protected:
	
	LastBossLeft( const std::string &readFileName, const math::Vector2 &enemyPos );

	// クラスの敵タイプ
	virtual const Common::TYPE_OBJECT GetTypeObject() const override{ return Common::TYPE_LAST_BOSS_RIGHT; }
		
	// 行動するときの回転量を取得
	virtual float GetRotateDefault() override{ return -20.0f; }
	virtual float GetRotActionFist() override{ return 0.0f; }
	virtual float GetRotActionSummon() override{ return -90.0f; }

	virtual math::Vector2 GetSummonEffectOffset() override{ return math::Vector2( 60.0f, 70.0f ); }

private:

	virtual ~LastBossLeft(void);
};

#endif