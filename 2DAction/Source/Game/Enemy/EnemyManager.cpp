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
		SetChildUnit( pEnemy );
		++currUniqueNo;
	}
}

bool EnemyManager::Init()
{

	return true;
}

void EnemyManager::Update()
{

}

bool EnemyManager::DieMain()
{

	return true;
}


