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

EnemyBoss *EnemyBoss::Create( const uint32_t &uniqueID )
{
	return NEW EnemyBoss( uniqueID );
}

EnemyBoss::EnemyBoss( const uint32_t &uniqueID )
	: EnemyBase( "EnemyBoss.json", uniqueID, Common::ENEMY_KIND_BOSS, 0 )
{
}

EnemyBoss::~EnemyBoss(void)
{
}

void EnemyBoss::ReduceDamage( Common::CMN_EVENT &eventId )
{
	switch( eventId.m_event ){
	case Common::EVENT_HIT_BULLET_PLAYER:
		// マシンガン攻撃は効きにくい( 0.9~4.5 )
		eventId.m_eventValue *= 0.7f - ( static_cast<float>( GetEnemyLevel()*0.02f ) );
		break;
	case Common::EVENT_HIT_BLADE_PLAYER:
		// 斬撃はダメージを増やす
		eventId.m_eventValue *= 1.5f;
		break;
	}
}




EnemySlimeKing *EnemySlimeKing::Create( const uint32_t &uniqueID )
{
	return NEW EnemySlimeKing( uniqueID );
}

EnemySlimeKing::EnemySlimeKing( const uint32_t &uniqueID )
	: EnemyBase( "EnemySlimeKing.json", uniqueID, Common::ENEMY_KIND_SLIME_KING, 0 )
{
}

EnemySlimeKing::~EnemySlimeKing(void)
{
}

const uint32_t EnemySlimeKing::GetEnemyDefaultSPD() const
{
	if( m_pEnemyAI && m_pEnemyAI->GetAIKind() == Common::AI_MOVE_PLAYER_SLIME_KING ){
		return 3;
	}
	return 1;
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

void EnemySlimeKing::ReduceDamage( Common::CMN_EVENT &eventId )
{
	switch( eventId.m_event ){
	case Common::EVENT_HIT_BULLET_PLAYER:
		// マシンガン攻撃は効きにくい( 0.9~4.5 )
		eventId.m_eventValue *= 0.9f - ( static_cast<float>( GetEnemyLevel()*0.05f ) );
		break;
	case Common::EVENT_HIT_BLADE_PLAYER:
		// 斬撃はダメージを増やす
		eventId.m_eventValue *= 1.2f;
		break;
	}
}
