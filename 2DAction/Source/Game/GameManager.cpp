/* ====================================================================== */
/**
 * @brief  敵の生成やアイテム生成を設定にならって管理するクラス
 *
 * @note
 *		
 */
/* ====================================================================== */

#include "System/SystemDefine.h"
#include "GameManager.h"
#include "GameRegister.h"
#include "System/picojson.h"
#include "Common/Utility/CommonGameUtility.h"


// アイテム、敵の出現頻度最大値
#define OBJECT_FREQUECY_MAX 9

GameManager *GameManager::CreateGameManager()
{
	return NEW GameManager();
}

GameManager::GameManager(void)
: TaskUnit("GameManager")
, m_gameTimer( 0 )
, m_gameTimeMax( 0 )
, m_enemyMax( 0 )	
, m_enemyFrequency( 0 )
, m_itemMax( 0 )
, m_itemFrequency( 0 )
{
}

GameManager::~GameManager(void)
{
}

bool GameManager::DieMain()
{
	return true;
}

bool GameManager::Init()
{
	// マップの環境取得
	LoadGameSettings("gameSettings.json");

	return true;
}

/* ================================================ */
/**
 * @brief	制限時間を過ぎたかどうか
 */
/* ================================================ */
bool GameManager::IsGameOver() const
{
	bool retVal = false;
	if( m_gameTimer > m_gameTimeMax ){
		retVal = true;
	}
	return retVal;
}

/* ================================================ */
/**
 * @brief	毎フレーム処理
 */
/* ================================================ */
void GameManager::Update()
{
	++m_gameTimer;

	EnemyManager *pEnemyManager = GameRegister::GetInstance()->UpdateManagerEnemy();
	ItemManager *pItemManager = GameRegister::GetInstance()->UpdateManagerItem();

	// 敵の生成
	if( IsCreateEnemy( m_enemyMax, m_enemyFrequency ) ){
		if( GetRandamValue( 1, 0 ) == 0){
			pEnemyManager->AddEnemy( Common::ENEMY_KIND_AAA );
		}
		else{
			pEnemyManager->AddEnemy( Common::ENEMY_KIND_BBB );
		}
	}
	// アイテムの生成
	if( IsCreateItem( m_itemMax, m_itemFrequency ) ){
		pItemManager->CreateItem( ItemObject::ITEM_KIND_RAPID_BULLET );
	}
}

/* ================================================ */
/**
 * @brief	環境情報取得
 */
/* ================================================ */
void GameManager::LoadGameSettings( const char *jsonFile )
{
	std::string path = JSON_OTHER_PATH;
	path += jsonFile;
	std::ifstream ifs(path.c_str());

	picojson::value root;
	picojson::parse( root, ifs);

	//!基本となる情報取得
	picojson::value timeData = root.get("gameTime");
	m_gameTimeMax	= static_cast<uint32_t>(timeData.get(0).get("TimeS").get<double>());
	m_gameTimeMax *= 60;
	
	picojson::value condData = root.get("gameCondition");
	m_enemyMax			= static_cast<uint32_t>(condData.get(0).get("EnemyMax").get<double>());
	m_enemyFrequency	= static_cast<uint32_t>(condData.get(0).get("frequency").get<double>());
	m_itemMax			= static_cast<uint32_t>(condData.get(1).get("ItemMax").get<double>());
	m_itemFrequency		= static_cast<uint32_t>(condData.get(1).get("frequency").get<double>());

	if( m_enemyFrequency > OBJECT_FREQUECY_MAX ){
		m_enemyFrequency = OBJECT_FREQUECY_MAX;
	}
	if( m_itemFrequency > OBJECT_FREQUECY_MAX ){
		m_itemFrequency = OBJECT_FREQUECY_MAX;
	}
}

bool GameManager::IsCreateEnemy( uint32_t enemyLimit, uint32_t frequency )
{
	bool isCreate = false;
	EnemyManager *pEnemyManager = GameRegister::GetInstance()->UpdateManagerEnemy();	
	// 敵の生成
	if( pEnemyManager ){
		uint32_t currEnemy = pEnemyManager->CountEnemy();
		uint32_t under = frequency + 1;
		if( currEnemy < (enemyLimit / under) || currEnemy == 0 ){
			// 一定の割合以下なら敵生成
			isCreate = true;
		}
		else if( currEnemy < enemyLimit ){
			// 出現頻度によって生成
			uint32_t rand = static_cast<uint32_t>( GetRandamValue( 1000, 0 ) );
			if( rand < ( 5 + (5*frequency) )){
				isCreate = true;	
			}
		}
	}
	return isCreate;
}

bool GameManager::IsCreateItem( uint32_t itemLimit, uint32_t frequency )
{
	bool isCreate = false;
	ItemManager *pEnemyManager = GameRegister::GetInstance()->UpdateManagerItem();	
	// 敵の生成
	if( pEnemyManager ){
		uint32_t currItem = pEnemyManager->CountItem();
		if( currItem <= itemLimit ){
			// 出現頻度によって生成
			uint32_t rand = static_cast<uint32_t>( GetRandamValue( 1000, 0 ) );
			if( rand < ( 5 + (5*frequency) )){
				isCreate = true;	
			}
		}
	}
	return isCreate;
}

