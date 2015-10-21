/* ====================================================================== */
/**
 * @brief  敵管理クラス
 *
 * @note
 *		
 */
/* ====================================================================== */

#include "EnemyDragon.h"
#include "Game/GameMap.h"
#include "Game/GameRegister.h"
#include "Game/GameManager.h"
#include "Flow/FlowManager.h"
#include "Flow/Process/FlowProcessBossEnemyDeath.h"

// 基準となる最低のHP
static const uint32_t DEFAULT_HP = 5000;

EnemyDragon *EnemyDragon::Create( const uint32_t &uniqueID, const uint32_t &enemyLevel, const math::Vector2 &enemyPos )
{
	return NEW EnemyDragon( uniqueID, enemyLevel, enemyPos );
}

EnemyDragon::EnemyDragon( const uint32_t &uniqueID, const uint32_t &enemyLevel, const math::Vector2 &enemyPos )
	: EnemyBase( "EnemyDragon.json", uniqueID, Common::ENEMY_KIND_DRAGON, enemyLevel, enemyPos )
{
}

EnemyDragon::~EnemyDragon(void)
{
}


const uint32_t EnemyDragon::GetEnemyDefaultHP() const
{
	return DEFAULT_HP + ( 800 * GetEnemyLevel() );
}

/* ================================================ */
/**
 * @brief	斬撃を受けた時の処理
 */
/* ================================================ */
void EnemyDragon::HitPlayreSlashing( const uint32_t &damageValue )
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
void EnemyDragon::EnemyDeath()
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

	// ラストステージならアイテムを落とす
	if( FlowManager::GetInstance()->GetCurrentFlowKind() == Common::FLOW_STAGE10 ){
		GameManager *pGameMan = GameRegister::GetInstance()->UpdateManagerGame();
		if( pGameMan ){
			pGameMan->CreateItem( Common::ITEM_KIND_LIFE_UP, GetDrawInfo().m_posOrigin );
		}
	}
}

/* ================================================ */
/**
 * @brief	ダメージ軽減処理
 */
/* ================================================ */
void EnemyDragon::ReduceDamage( Common::CMN_EVENT &eventId )
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
