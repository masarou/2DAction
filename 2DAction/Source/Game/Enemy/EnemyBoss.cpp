/* ====================================================================== */
/**
 * @brief  敵管理クラス
 *
 * @note
 *		
 */
/* ====================================================================== */

#include "EnemyBoss.h"
#include "Game/GameMap.h"
#include "Game/GameRegister.h"
#include "Flow/FlowManager.h"
#include "Flow/Process/FlowProcessBossEnemyDeath.h"

EnemyBoss *EnemyBoss::Create( const uint32_t &uniqueID, const uint32_t &enemyLevel, const math::Vector2 &enemyPos )
{
	return NEW EnemyBoss( uniqueID, enemyLevel, enemyPos );
}

EnemyBoss::EnemyBoss( const uint32_t &uniqueID, const uint32_t &enemyLevel, const math::Vector2 &enemyPos )
	: EnemyBase( "EnemyBoss.json", uniqueID, Common::ENEMY_KIND_BOSS, enemyLevel, enemyPos )
{
}

EnemyBoss::~EnemyBoss(void)
{
}

/* ================================================ */
/**
 * @brief	斬撃を受けた時の処理
 */
/* ================================================ */
void EnemyBoss::HitPlayreSlashing( const uint32_t &damageValue )
{
	// スタンはなし
	SetStunTime( 10 );
	GameEffect::CreateEffect( GameEffect::EFFECT_SLASHING_HIT, m_drawTexture.m_pTex2D->GetDrawInfo().m_posOrigin );
	UpdateEnemyDamage( damageValue );
}

/* ================================================ */
/**
 * @brief	斬撃を与えた時のダメージ量を返す
 */
/* ================================================ */
uint32_t EnemyBoss::GetSlashingDamage() const
{
	return 20 + ( 6 * GetEnemyLevel() );
}

/* ================================================ */
/**
 * @brief	死亡時に呼ばれる
 * 			ボスクラス特有処理セット
 */
/* ================================================ */
void EnemyBoss::EnemyDeath()
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
}

/* ================================================ */
/**
 * @brief	ダメージ軽減処理
 */
/* ================================================ */
void EnemyBoss::ReduceDamage( Common::CMN_EVENT &eventId )
{
	float levelReduce = static_cast<float>( GetEnemyLevel()*0.02f );
	switch( eventId.m_event ){
	case Common::EVENT_HIT_BULLET_PLAYER:
		// マシンガン攻撃は効きにくい( 0.9~4.5 )
		eventId.m_eventValue *= 0.7f - levelReduce;
		break;
	case Common::EVENT_HIT_BLADE_PLAYER:
		// 斬撃はダメージを増やす
		eventId.m_eventValue *= 1.3f - levelReduce;
		break;
	}
}




EnemySlimeKing *EnemySlimeKing::Create( const uint32_t &uniqueID, const uint32_t &enemyLevel, const math::Vector2 &enemyPos )
{
	return NEW EnemySlimeKing( uniqueID, enemyLevel, enemyPos );
}

EnemySlimeKing::EnemySlimeKing( const uint32_t &uniqueID, const uint32_t &enemyLevel, const math::Vector2 &enemyPos )
	: EnemyBase( "EnemySlimeKing.json", uniqueID, Common::ENEMY_KIND_SLIME_KING, enemyLevel, enemyPos )
{
}

EnemySlimeKing::~EnemySlimeKing(void)
{
}

const float EnemySlimeKing::GetEnemyDefaultSPD() const
{
	if( m_pEnemyAI && m_pEnemyAI->GetAIKind() == Common::AI_MOVE_PLAYER_SLIME_KING ){
		return 3.0f;
	}
	return 1.0f;
}

void EnemySlimeKing::EnemyDeath()
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
}


const uint32_t EnemySlimeKing::GetEnemyDefaultHP() const
{
	return 3500 + ( 400 * GetEnemyLevel() );
}

void EnemySlimeKing::ReduceDamage( Common::CMN_EVENT &eventId )
{
	float levelReduce = static_cast<float>( GetEnemyLevel()*0.05f );
	switch( eventId.m_event ){
	case Common::EVENT_HIT_BULLET_PLAYER:
		// マシンガン攻撃は効きにくい( 0.9~4.5 )
		eventId.m_eventValue *= 0.9f - levelReduce;
		break;
	case Common::EVENT_HIT_BLADE_PLAYER:
		// 斬撃はダメージを増やす
		eventId.m_eventValue *= 1.2f - levelReduce;
		break;
	}
}
