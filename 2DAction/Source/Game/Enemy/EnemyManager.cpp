/* ====================================================================== */
/**
 * @brief  �G�Ǘ��N���X
 *
 * @note
 *		
 */
/* ====================================================================== */

#include "EnemyManager.h"
#include "EnemySlime.h"
#include "EnemyAnotherSlime.h"
#include "EnemyAhriman.h"
#include "EnemyCow.h"
#include "EnemySlimeKing.h"
#include "EnemyRanger.h"
#include "EnemyWizard.h"
#include "EnemyDragon.h"
#include "EnemyLastBoss.h"
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
		if( !m_enemyArray.at(i)->m_pEnemyAI ){
			DEBUG_ASSERT( 0, "�GAI��NULL");
			continue;
		}
		if( m_enemyArray.at(i)->m_pEnemyAI->GetAIKind() == Common::AI_MOVE_PLAYER ){
			++m_moveToPlayerNum;
		}
	}
}

bool EnemyManager::DieMain()
{
	// �ێ����Ă���N���X�����ׂď��O
	m_enemyArray.clear();

	// �G�N���X�����L�Ŏg�p���Ă�����̂�clear
	EnemyAIBase::ClearAttackMaterial();

	return true;
}

/* ================================================ */
/**
 * @brief	�G�N���X�̒ǉ��Ə��O
 */
/* ================================================ */
void EnemyManager::AddEnemy( const Common::ENEMY_KIND &kind, const uint32_t &enemyLevel, const math::Vector2 &enemyPos )
{
	static uint32_t currUniqueNo = 0;
	EnemyBase *pEnemy = NULL;
	switch( kind ){
	case Common::ENEMY_KIND_SLIME:
		pEnemy = EnemySlime::Create( currUniqueNo, enemyLevel, enemyPos );
		break;
	case Common::ENEMY_KIND_SLIME_ANOTHER:
		pEnemy = EnemyAnotherSlime::Create( currUniqueNo, enemyLevel, enemyPos );
		break;
	case Common::ENEMY_KIND_AHRIMAN:
		pEnemy = EnemyBBB::Create( currUniqueNo, enemyLevel, enemyPos );
		break;
	case Common::ENEMY_KIND_COW:
		pEnemy = EnemyCCC::Create( currUniqueNo, enemyLevel, enemyPos );
		break;
	case Common::ENEMY_KIND_RANGER:
		pEnemy = EnemyRanger::Create( currUniqueNo, enemyLevel, enemyPos );
		break;
	case Common::ENEMY_KIND_SLIME_KING:
		pEnemy = EnemySlimeKing::Create( currUniqueNo, enemyLevel, enemyPos );
		break;
	case Common::ENEMY_KIND_WIZARD:
		pEnemy = EnemyWizard::Create( currUniqueNo, enemyLevel, enemyPos );
		break;
	case Common::ENEMY_KIND_DRAGON:
		pEnemy = EnemyDragon::Create( currUniqueNo, enemyLevel, enemyPos );
		break;
	case Common::ENEMY_KIND_LAST_BOSS:
		pEnemy = LastBoss::Create( currUniqueNo, enemyLevel, enemyPos );
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

/* ================================================ */
/**
 * @brief	�w�荀�ڂ̓G���J�E���g
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
 * @brief	SystemMessageUnit�̃��j�[�NID����G�N���X���擾
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
