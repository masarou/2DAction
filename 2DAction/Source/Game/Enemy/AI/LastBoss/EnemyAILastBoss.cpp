/* ====================================================================== */
/**
 * @brief  ラスボスAI
 *
 * @note
 *		
 */
/* ====================================================================== */

#include "Game/GameRegister.h"
#include "Game/Enemy/EnemyManager.h"

#include "EnemyAILastBoss.h"
#include "Common/Utility/CommonGameUtility.h"
#include "System/Draw2D/SystemDraw2DResource.h"
#include "Game/Effect/GameEffect.h"

EnemyAILastBoss *EnemyAILastBoss::Create()
{
	EnemyAILastBoss *tmpAI = NEW EnemyAILastBoss();
	return tmpAI;
}

EnemyAILastBoss::EnemyAILastBoss(void)
: m_pLeftHand( NULL )
, m_pRightHand( NULL )
, m_moveBasicPos( math::Vector2() )
{
}


EnemyAILastBoss::~EnemyAILastBoss(void)
{
}

bool EnemyAILastBoss::InitAI()
{
	m_pRightHand	= LastBossRight::Create( "EnemyLastBossRight.json", math::Vector2() );
	m_pLeftHand		= LastBossLeft::Create( "EnemyLastBossLeft.json", math::Vector2() );
	return true;
}

void EnemyAILastBoss::ExecMain( TEX_DRAW_INFO &enemyInfo, ACTION_ARRAY &actionInfo )
{
	// 最初を除き基本的にm_moveBasicPosで上書き
	// その後、ふわふわ浮いている分の数値を足してやる
	if( m_moveBasicPos == math::Vector2() ){
		m_moveBasicPos = enemyInfo.m_posOrigin;
	}
	else{
		enemyInfo.m_posOrigin = m_moveBasicPos;
	}

	// 両腕の更新(新たな行動タイプ設定や、定位置の更新)
	ExecHandsUpdate( enemyInfo );

	// ふわふわ浮いている分の数値
	static uint32_t counter = 0;
	enemyInfo.m_posOrigin.y += math::Sin(counter) * 10.0f;
	++counter;

}

void EnemyAILastBoss::EnemyIsDead()
{
	// 両手クラス死亡
	m_pRightHand->StartDie();
}




/* ================================================ */
/**
 * @brief	両腕更新
 */
/* ================================================ */
void EnemyAILastBoss::ExecHandsUpdate( TEX_DRAW_INFO &enemyInfo )
{
	// 基準点更新
	math::Vector2 letfPos = enemyInfo.m_posOrigin;
	letfPos += math::Vector2( 150.0f, 50.0f );
	math::Vector2 rightPos = enemyInfo.m_posOrigin;
	rightPos += math::Vector2( -150.0f, 50.0f );

	if( m_pRightHand ){
		m_pRightHand->SetBasicPos( rightPos );
	}
	if( m_pLeftHand ){
		m_pLeftHand->SetBasicPos( letfPos );
	}

}




/* ====================================================================== */
/**
 * @brief  ラスボス右手左手クラス
 *
 * @note
 *		
 */
/* ====================================================================== */
LastBossHand::LastBossHand( const std::string &readFileName, const math::Vector2 &enemyPos )
: TaskUnit("Enemy")
, Collision2DUnit( readFileName.c_str() )
, m_basicPos( math::Vector2() )
, m_currAction( ACTION_MAX )
, m_nextAction( ACTION_MAX )
{


}

LastBossHand::~LastBossHand()
{

}

void LastBossHand::SetBasicPos( math::Vector2 basicPos )
{
	m_basicPos = basicPos;
}

void LastBossHand::Update()
{
	TEX_DRAW_INFO &drawInfo = m_drawTexture.m_pTex2D->UpdateDrawInfo();
	drawInfo.m_posOrigin = m_basicPos;
}


void LastBossHand::DrawUpdate()
{
	if( m_drawTexture.m_pTex2D ){
		m_drawTexture.m_pTex2D->DrawUpdate2D();
	}
}




LastBossRight *LastBossRight::Create( const std::string &readFileName, const math::Vector2 &enemyPos )
{
	return NEW LastBossRight( readFileName, enemyPos );
}

LastBossRight::LastBossRight( const std::string &readFileName, const math::Vector2 &enemyPos )
: LastBossHand( readFileName, enemyPos )
{

}

LastBossRight::~LastBossRight()
{
}


LastBossLeft *LastBossLeft::Create( const std::string &readFileName, const math::Vector2 &enemyPos )
{
	return NEW LastBossLeft( readFileName, enemyPos );
}

LastBossLeft::LastBossLeft( const std::string &readFileName, const math::Vector2 &enemyPos )
: LastBossHand( readFileName, enemyPos )
{

}

LastBossLeft::~LastBossLeft()
{
}