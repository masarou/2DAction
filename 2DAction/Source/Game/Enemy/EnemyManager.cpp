/* ====================================================================== */
/**
 * @brief  �G�Ǘ��N���X
 *
 * @note
 *		
 */
/* ====================================================================== */

#include "EnemyManager.h"
#include "EnemyAAA.h"
#include "EnemyBBB.h"
#include "EnemyCCC.h"
#include "EnemyBoss.h"
#include "AI/EnemyAIBase.h"
#include "Game/Attack/Bullet.h"
#include "Game/Player/GamePlayer.h"
#include "Game/GameRegister.h"
#include "Common/Utility/CommonGameUtility.h"

EnemyManager *EnemyManager::CreateEnemyManager()
{
	return NEW EnemyManager();
}

EnemyManager::EnemyManager(void)
: TaskUnit("EnemyManager")
{
}

EnemyManager::~EnemyManager(void)
{
}

bool EnemyManager::DieMain()
{
	// �ێ����Ă���N���X�����ׂď��O
	m_enemyArray.clear();

	// �G�N���X�����L�Ŏg�p���Ă�����̂�clear
	EnemyAIBase::ClearAttackGun();

	return true;
}

/* ================================================ */
/**
 * @brief	�G�N���X�̒ǉ��Ə��O
 */
/* ================================================ */
void EnemyManager::AddEnemy( const Common::ENEMY_KIND &kind )
{
	static uint32_t currUniqueNo = 0;
	EnemyBase *pEnemy = NULL;
	switch( kind ){
	case Common::ENEMY_KIND_AAA:
		pEnemy = EnemyAAA::Create( currUniqueNo );
		break;
	case Common::ENEMY_KIND_BBB:
		pEnemy = EnemyBBB::Create( currUniqueNo );
		break;
	case Common::ENEMY_KIND_CCC:
		pEnemy = EnemyCCC::Create( currUniqueNo );
		break;
	case Common::ENEMY_KIND_BOSS:
		pEnemy = EnemyBoss::Create( currUniqueNo );
		break;

	default:
		DEBUG_ASSERT( 0,  "�G�̎�ނ��z��O" );
		break;
	}
	if( pEnemy ){
		m_enemyArray.push_back( pEnemy );
		++currUniqueNo;
	}
}

void EnemyManager::RemoveEnemy( EnemyBase *removeEnemy )
{
	for( auto it = m_enemyArray.begin(); it != m_enemyArray.end() ; ++it ){
		if( (*it) == removeEnemy ){
			m_enemyArray.erase(it);
			break;
		}
	}
}

uint32_t EnemyManager::CountEnemy( const Common::ENEMY_KIND &kind ) const
{
	uint32_t retVal = 0;
	if( kind == Common::ENEMY_KIND_MAX ){
		retVal = m_enemyArray.size();
	}
	else{
		for( uint32_t i = 0; i < m_enemyArray.size() ; ++i ){
			if( kind == m_enemyArray.at(i)->GetKind() ){
				++retVal;
			}
		}

	}

	return retVal;
}
