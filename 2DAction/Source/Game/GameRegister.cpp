/* ====================================================================== */
/**
 * @brief  プレイヤー、敵などのユニーククラスを持つ
 *
 * @note
 *		
 */
/* ====================================================================== */

#include "GameRegister.h"

GameRegister *GameRegister::s_pInstance = NULL;

GameRegister *GameRegister::CreateInstance()
{
	if( !s_pInstance ){
		s_pInstance = NEW GameRegister();
	}
	return s_pInstance;
}


GameRegister *GameRegister::GetInstance()
{
	return s_pInstance;
}

GameRegister::GameRegister(void)
: m_pPlayer( NULL )
, m_pEnemyManager( NULL )
, m_pMap( NULL )
, m_pItemManager( NULL )
{
	m_pPlayer = GamePlayer::CreatePlayer();
	m_pEnemyManager = EnemyManager::CreateEnemyManager();
	m_pMap = GameMap::CreateGameMap();
	m_pItemManager = ItemManager::CreateItemManager();
}

void GameRegister::DeleteRegister()
{
	SAFE_DELETE( s_pInstance );
}

GameRegister::~GameRegister(void)
{
}

/* ================================================ */
/**
 * @brief	各種インスタンス取得関数
 */
/* ================================================ */
GamePlayer *GameRegister::UpdatePlayer()
{
	return m_pPlayer;
}

EnemyManager *GameRegister::UpdateManagerEnemy()
{
	return m_pEnemyManager;
}

GameMap *GameRegister::UpdateGameMap()
{
	return m_pMap;
}

ItemManager *GameRegister::UpdateManagerItem()
{
	return m_pItemManager;
}

const GamePlayer *GameRegister::GetPlayer() const
{
	return m_pPlayer;
}

const EnemyManager *GameRegister::GetManagerEnemy() const
{
	return m_pEnemyManager;
}

const GameMap *GameRegister::GetGameMap() const
{
	return m_pMap;
}

const ItemManager *GameRegister::GetManagerItem() const
{
	return m_pItemManager;
}
