/* ====================================================================== */
/**
 * @brief  敵AIの通常クラス(プレイヤー未発見)
 *
 * @note
 *		デフォルトAI
 */
/* ====================================================================== */

#include "EnemyAISearch.h"
#include "Common/Utility/CommonGameUtility.h"

EnemyAISearch *EnemyAISearch::Create()
{
	EnemyAISearch *tmpAI = NEW EnemyAISearch();
	return tmpAI;
}

EnemyAISearch::EnemyAISearch(void)
: m_circleRadius( 30 )
, m_circleDistance( 100 )
{
}


EnemyAISearch::~EnemyAISearch(void)
{
}

bool EnemyAISearch::InitAI()
{
	return true;
}

void EnemyAISearch::ExecMain( TEX_DRAW_INFO &enemyInfo )
{
	// 目標となる地点を設定
	math::Vector2 vec = math::Vector2( GetRandamValueFloat( 100, -100 ), GetRandamValueFloat( 100, -100 ) );
	vec.Normalize();
	vec *= static_cast<float>(m_circleRadius);

	vec += enemyInfo.m_posOrigin;
	if( m_enemyMine ){
		const math::Vector2 &eye = m_enemyMine->GetEnemyEyeSight();
		vec += eye * static_cast<float>(m_circleDistance);
	}
	vec -= enemyInfo.m_posOrigin;
	vec.Normalize();

	math::Vector2 nextPos = enemyInfo.m_posOrigin + (vec * 2.0f);
	if( GetMapHeight( nextPos ) == 0 ){
		enemyInfo.m_posOrigin += vec * 2.0f;
	}

	m_enemyMine->SetEnemyEyeSight( vec );

}

