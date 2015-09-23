/* ====================================================================== */
/**
 * @brief  敵管理クラス
 *
 * @note
 *		
 */
/* ====================================================================== */

#include "EnemyManager.h"
#include "EnemySlime.h"
#include "EnemyAhriman.h"
#include "EnemyCow.h"
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
, m_moveToPlayerNum( 0 )
{
}

EnemyManager::~EnemyManager(void)
{
}

void EnemyManager::Update()
{
	m_moveToPlayerNum = 0;
	for( uint32_t i = 0; i < m_enemyArray.size() ; ++i ){
		if( m_enemyArray.at(i)->m_pEnemyAI->GetAIKind() == Common::AI_MOVE_PLAYER ){
			++m_moveToPlayerNum;
		}
	}
}

bool EnemyManager::DieMain()
{
	// 保持しているクラスをすべて除外
	m_enemyArray.clear();

	// 敵クラスが共有で使用しているものをclear
	EnemyAIBase::ClearAttackMaterial();

	return true;
}

/* ================================================ */
/**
 * @brief	敵クラスの追加と除外
 */
/* ================================================ */
void EnemyManager::AddEnemy( const Common::ENEMY_KIND &kind, const uint32_t &enemyLevel, const math::Vector2 &enemyPos )
{
	static uint32_t currUniqueNo = 0;
	EnemyBase *pEnemy = NULL;
	switch( kind ){
	case Common::ENEMY_KIND_SLIME:
		pEnemy = EnemySlime::Create( enemyLevel, currUniqueNo, enemyPos );
		break;
	case Common::ENEMY_KIND_AHRIMAN:
		pEnemy = EnemyBBB::Create( enemyLevel, currUniqueNo, enemyPos );
		break;
	case Common::ENEMY_KIND_COW:
		pEnemy = EnemyCCC::Create( enemyLevel, currUniqueNo, enemyPos );
		break;
	case Common::ENEMY_KIND_BOSS:
		pEnemy = EnemyBoss::Create( currUniqueNo );
		break;
	case Common::ENEMY_KIND_SLIME_KING:
		pEnemy = EnemySlimeKing::Create( currUniqueNo );
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

/* ================================================ */
/**
 * @brief	指定項目の敵をカウント
 */
/* ================================================ */
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

/* ================================================ */
/**
 * @brief	SystemMessageUnitのユニークIDから敵クラスを取得
 */
/* ================================================ */
const EnemyBase *EnemyManager::GetEnemyBymessageUniqueID( uint32_t messageUnitID )
{
	for( uint32_t i = 0; i < m_enemyArray.size() ; ++i ){
		if( m_enemyArray.at(i)->GetUniqueId() == messageUnitID){
			return m_enemyArray.at(i);
		}
	}
	return NULL;
}
