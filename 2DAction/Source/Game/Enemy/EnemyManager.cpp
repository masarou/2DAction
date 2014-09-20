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
//#include "EnemyBBB.h"
//#include "EnemyCCC.h"
//#include "EnemyDDD.h"
#include "Game/Player/AttackGun/Bullet.h"

EnemyManager *EnemyManager::m_pInstance = NULL;

EnemyManager *EnemyManager::GetInstance()
{
	if( !m_pInstance ){
		m_pInstance = NEW EnemyManager();
	}
	return m_pInstance;
}

EnemyManager::EnemyManager(void)
{
}

EnemyManager::~EnemyManager(void)
{
}

void EnemyManager::CreateEnemy( Common::ENEMY_KIND kind )
{
	static uint32_t currUniqueNo = 0;
	EnemyBase *pEnemy = NULL;
	switch( kind ){
	case Common::KIND_AAA:
		pEnemy = EnemyAAA::Create( currUniqueNo );
		break;
	case Common::KIND_BBB:
		pEnemy = EnemyAAA::Create( currUniqueNo );
		break;
	case Common::KIND_CCC:
		pEnemy = EnemyAAA::Create( currUniqueNo );
		break;
	case Common::KIND_DDD:
		pEnemy = EnemyAAA::Create( currUniqueNo );
		break;

	default:
		DEBUG_ASSERT( "敵の種類が想定外" );
		break;
	}
	if( pEnemy ){
		m_enemyArray.push_back( pEnemy );
		++currUniqueNo;
	}
}

void EnemyManager::DeleteEnemy( uint32_t uniqueNumber )
{
	std::vector<EnemyBase*>::iterator it = m_enemyArray.begin();
	for( uint32_t i = 0; i < m_enemyArray.size() ; ++i ){
		if( (*it)->GetUniqueNumber() == uniqueNumber ){
			EnemyBase *pTmp = ( *it );
			m_enemyArray.erase( it );
			SAFE_DELETE( pTmp );
			break;
		}
		++it;
	}

}

void EnemyManager::CheckCollision( Bullet *bullet )
{
	for( uint32_t i = 0; i < m_enemyArray.size() ; ++i){
		if( 0 ){
			Common::CMN_EVENT eventInfo;
			eventInfo.m_event = Common::EVENT_HIT_BULLET;
			eventInfo.m_eventValue = m_enemyArray.at(i)->GetUniqueNumber();
			AddEvent( eventInfo );
		}
	}
}


// 派生先でのメッセージ処理
void EnemyManager::EventUpdate( const Common::CMN_EVENT &eventId )
{
	switch( eventId.m_event ){
	case Common::EVENT_HIT_BULLET:
		{
			// 弾が当たったことをおしえてやる
			for(uint32_t i = 0; i < m_enemyArray.size() ; ++i ){
				if( m_enemyArray.at(i)->GetUniqueNumber() == eventId.m_eventValue ){
					m_enemyArray.at(i)->EventUpdate( eventId );
				}
			}
		}
		break;

	default:

		break;
	}
}