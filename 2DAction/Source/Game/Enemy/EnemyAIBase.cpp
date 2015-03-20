
/* ====================================================================== */
/**
 * @brief  “GAI‚ÌBaseƒNƒ‰ƒX
 *
 * @note
 *		
 */
/* ====================================================================== */

#include "EnemyAIBase.h"
#include "Game/Enemy/EnemyBase.h"
#include "Game/Attack/GameAttackGun.h"

EnemyAIBase::EnemyAIBase(void)
: m_enemyMine( NULL )
{
}

EnemyAIBase::EnemyAIBase(EnemyBase *enemyMine)
: m_enemyMine( enemyMine )
, m_isReady( false )
{
}

EnemyAIBase::~EnemyAIBase(void)
{
}

void EnemyAIBase::SetThingingEnemy( EnemyBase *enemyMine )
{
	m_enemyMine = enemyMine;
}



void EnemyAIBase::Exec( TEX_DRAW_INFO &enemyInfo, ACTION_INFO &actionInfo )
{
	if( !m_isReady ){
		m_isReady = InitAI();
	}
	else{
		ExecMain( enemyInfo, actionInfo );
	}
}


// “GƒLƒƒƒ‰‚»‚Ì‚à‚Ì‚ÉŠÖ‚·‚éget,setŠÖ”
void EnemyAIBase::SetEnemyAnim( const std::string &animTag )
{
	if( m_enemyMine && m_enemyMine->m_drawTexture.m_pTex2D ){
		m_enemyMine->m_drawTexture.m_pTex2D->SetAnim( animTag );
	}
}

std::string EnemyAIBase::GetEnemyAnim()
{
	std::string animTag = "";
	if( m_enemyMine && m_enemyMine->m_drawTexture.m_pTex2D ){
		animTag = m_enemyMine->m_drawTexture.m_pTex2D->GetPlayAnim();
	}
	return animTag;
}


void EnemyAIBase::SetEnemyEyeSight( math::Vector2 &eye )
{
	if( m_enemyMine ){
		eye.Normalize();
		m_enemyMine->m_eye = eye;
	}
}

math::Vector2 EnemyAIBase::GetEnemyEyeSight() const
{
	if( m_enemyMine ){
		return m_enemyMine->GetEnemyEyeSight();
	}
	return DEFAULT_VECTOR2;
}

void EnemyAIBase::ChangeEnemyAI( Common::ENEMY_AI nextAI )
{
	if( !m_enemyMine ){
		return;
	}

	if( m_enemyMine->m_nextAI != nextAI ){
		m_enemyMine->m_nextAI = nextAI;
	}
}

Common::ENEMY_KIND EnemyAIBase::GetEnemyKind() const
{
	if( m_enemyMine ){
		return m_enemyMine->GetKind();
	}
	return Common::ENEMY_KIND_MAX;
}

const std::string &EnemyAIBase::GetEnemyJsonName() const
{
	return m_enemyMine->GetDrawInfo().m_fileName;
}
