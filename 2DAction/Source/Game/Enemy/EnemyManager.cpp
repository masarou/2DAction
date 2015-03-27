/* ====================================================================== */
/**
 * @brief  敵管理クラス
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
	// 保持しているクラスをすべて除外
	m_enemyArray.clear();

	// 敵クラスが共有で使用しているものをclear
	EnemyAIBase::ClearAttackGun();

	return true;
}

/* ================================================ */
/**
 * @brief	敵クラスの追加と除外
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
		DEBUG_ASSERT( 0,  "敵の種類が想定外" );
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
