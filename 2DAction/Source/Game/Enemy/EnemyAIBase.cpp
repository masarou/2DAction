
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
