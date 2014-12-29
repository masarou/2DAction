
#include "EnemyAIBase.h"
#include "Game/Enemy/EnemyBase.h"

//EnemyAIBase *EnemyAIBase::Create( EnemyBase *enemyMine = NULL )
//{
//
//	if( enemyMine ){
//		return 
//	}
//}

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



void EnemyAIBase::Exec( TEX_DRAW_INFO &enemyInfo )
{
	if( !m_isReady ){
		m_isReady = InitAI();
	}
	else{
		ExecMain( enemyInfo );
	}
}


// “GƒLƒƒƒ‰‚»‚Ì‚à‚Ì‚ÉŠÖ‚·‚éget,setŠÖ”
void EnemyAIBase::ChangeEnemyAI( Common::ENEMY_AI nextAI )
{
	if( m_enemyMine ){
		m_enemyMine->m_nextAI = nextAI;
	}
}

void EnemyAIBase::SetEnemyAnim( const std::string &animTag )
{
	if( m_enemyMine && m_enemyMine->m_textureEnemy.m_pTex2D ){
		m_enemyMine->m_textureEnemy.m_pTex2D->SetAnim( animTag );
	}
}

std::string EnemyAIBase::GetEnemyAnim()
{
	std::string animTag = "";
	if( m_enemyMine && m_enemyMine->m_textureEnemy.m_pTex2D ){
		animTag = m_enemyMine->m_textureEnemy.m_pTex2D->GetPlayAnim();
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

const math::Vector2 &EnemyAIBase::GetEnemyEyeSight() const
{
	if( m_enemyMine ){
		return m_enemyMine->m_eye;
	}
	return DEFAULT_VECTOR2;
}
