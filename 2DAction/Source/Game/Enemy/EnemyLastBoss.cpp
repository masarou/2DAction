/* ====================================================================== */
/**
 * @brief  ラスボス管理クラス
 *
 * @note
 *		
 */
/* ====================================================================== */

#include "EnemyLastBoss.h"
#include "Game/GameMap.h"
#include "Game/GameRegister.h"
#include "Game/GameRecorder.h"
#include "Flow/FlowManager.h"
#include "Flow/Process/FlowProcessBossEnemyDeath.h"

// 基準となる最低のHP
static const uint32_t DEFAULT_HP = 10000;

LastBoss *LastBoss::Create( const uint32_t &uniqueID, const uint32_t &enemyLevel, const math::Vector2 &enemyPos )
{
	return NEW LastBoss( uniqueID, enemyLevel, enemyPos );
}

LastBoss::LastBoss( const uint32_t &uniqueID, const uint32_t &enemyLevel, const math::Vector2 &enemyPos )
	: EnemyBase( "EnemyLastBoss.json", uniqueID, Common::ENEMY_KIND_LAST_BOSS, enemyLevel, enemyPos )
{
}

LastBoss::~LastBoss(void)
{
}

bool LastBoss::InitMain()
{
	if( m_drawTexture.m_pTex2D ){
		m_drawTexture.m_pTex2D->UpdateDrawInfo().m_prioity = Common::PRIORITY_ABOVE_NORMAL;
	}
	return true;
}

const uint32_t LastBoss::GetEnemyDefaultHP() const
{
	return DEFAULT_HP + ( 800 * GetEnemyLevel() );
}

/* ================================================ */
/**
 * @brief	斬撃を受けた時の処理
 */
/* ================================================ */
void LastBoss::HitPlayreSlashing( const uint32_t &damageValue )
{
	// スタンはなし
	SetStunTime( 10 );
	GameEffect::CreateEffect( GameEffect::EFFECT_SLASHING_HIT, m_drawTexture.m_pTex2D->GetDrawInfo().m_posOrigin );
	UpdateEnemyDamage( damageValue );
}

/* ================================================ */
/**
 * @brief	死亡時に呼ばれる
 * 			ボスクラス特有処理セット
 */
/* ================================================ */
void LastBoss::EnemyDeath()
{
	// 死亡したので専用演出再生
	ProcessBossEnemyDeath *pDeathEffect = ProcessBossEnemyDeath::Create( m_drawTexture.m_pTex2D->GetDrawInfo().m_posOrigin );
	FlowManager *pFlowMan = FlowManager::GetInstance();
	if( pFlowMan && pDeathEffect ){
		pFlowMan->SetupSpecialEffect( pDeathEffect );
	}
	if( m_drawTexture.m_pTex2D ){
		m_drawTexture.m_pTex2D->SetAnim( "death" );
	}

	// 全クリアフラグON
	GameRecorder::GetInstance()->SetClearAll();
}

/* ================================================ */
/**
 * @brief	ダメージ軽減処理
 */
/* ================================================ */
void LastBoss::ReduceDamage( Common::CMN_EVENT &eventId )
{
	float levelReduce = static_cast<float>( GetEnemyLevel()*0.02f );
	switch( eventId.m_event ){
	case Common::EVENT_HIT_BULLET_PLAYER:
		// マシンガン攻撃は効きにくい( 0.9~4.5 )
		eventId.m_eventValue *= 0.7f - levelReduce;
		break;
	case Common::EVENT_HIT_BLADE_PLAYER:
		// 斬撃はダメージを増やす
		eventId.m_eventValue *= 1.0f - levelReduce;
		break;
	}

	// 現存しているユニークモンスターの数で変化
	EnemyManager *pEnemyManager = GameRegister::GetInstance()->UpdateManagerEnemy();	
	// 敵の生成
	if( pEnemyManager ){
		uint32_t countEnemy = 0;
		for( uint32_t i = 0; i < NUMBEROF(Common::s_uniqueEnemyKind) ; ++i ){
			countEnemy += pEnemyManager->CountEnemy( Common::s_uniqueEnemyKind[i] );
		}
		if( countEnemy == 1 ){
			eventId.m_eventValue *= 0.5f;
		}
		else if( countEnemy >= 2 ){
			eventId.m_eventValue = 0;
			// ダメージ無効エフェクト
			GameEffect::CreateEffect( GameEffect::EFFECT_INVALID_DAMAGE, GetDrawInfo().m_posOrigin );
			// 無効SE
			SoundManager::GetInstance()->PlaySE( "invalidDamage" );
		}
	}
}

