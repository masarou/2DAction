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

GameManager *GameManager::CreateGameManager( const Common::GAME_FLOW &currentKind )
{
	return NEW GameManager( currentKind );
}

GameManager::GameManager( const Common::GAME_FLOW &currentKind )
: TaskUnit("GameManager")
, m_type( TYPE_MAX )
, m_gameTimer( 0 )
, m_gameTimeMax( 0 )
, m_destroyNum( 0 )
, m_destroyMax( 0 )
, m_enemyMax( 0 )	
, m_enemyFrequency( 0 )
, m_itemMax( 0 )
, m_itemFrequency( 0 )
{
	switch( currentKind ){
		default:
			DEBUG_ASSERT( 0, "想定外の箇所でゲーム設定ファイルが読み込まれている");
			/* fall-through */
		case Common::FLOW_STAGE01:
			m_settingFileStr = "gameSettings01.json";
			break;
		case Common::FLOW_STAGE02:
			m_settingFileStr = "gameSettings02.json";
			break;
		case Common::FLOW_STAGE03:
			m_settingFileStr = "gameSettings03.json";
			break;
	}
	// マップの環境取得
	LoadGameSettings( m_settingFileStr.c_str() );
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
	return true;
}

/* ================================================ */
/**
 * @brief	ステージクリア条件を満たしたかどうか
 */
/* ================================================ */
bool GameManager::IsGameOver() const
{
	bool retVal = false;
	if( m_type == TYPE_TIME ){
		if( m_gameTimer > m_gameTimeMax ){
			retVal = true;
		}
	}
	else if( m_type == TYPE_DESTROY ){
		if( m_destroyNum >= m_destroyMax ){
			retVal = true;
		}
	}
	return retVal;
}

/* ================================================ */
/**
 * @brief	倒した敵の数をカウント
 */
/* ================================================ */
void GameManager::AddDestroyCount()
{
	++m_destroyNum;
}

/* ================================================ */
/**
 * @brief	ステージタイプ取得
 */
/* ================================================ */
const GameManager::STAGE_TYPE GameManager::GetStageType() const
{
	return m_type;
}

/* ================================================ */
/**
 * @brief	残り時間or殲滅数の取得
 */
/* ================================================ */
const uint32_t GameManager::GetGameLeftTimeByFrame() const
{
	uint32_t retVal = 0;
	if( m_gameTimeMax > m_gameTimer ){
		retVal = m_gameTimeMax - m_gameTimer;
	}

	return retVal;
}
const uint32_t GameManager::GetGameLeftTimeBySec() const
{
	uint32_t retVal = 0;
	if( m_gameTimeMax > m_gameTimer ){
		retVal = m_gameTimeMax - m_gameTimer;
		retVal += 59;
		retVal /= 60;
	}

	return retVal;
}
const uint32_t GameManager::GetGameLeftDestroy() const
{
	uint32_t retVal = 0;
	if( m_gameTimeMax > m_gameTimer ){
		retVal = m_destroyMax - m_destroyNum;
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
		pEnemyManager->AddEnemy( Common::ENEMY_KIND_BOSS );
		return;
		if( Utility::GetRandamValue( 5, 0 ) != 0 ){
			pEnemyManager->AddEnemy( Common::ENEMY_KIND_AAA );
		}
		else if( Utility::GetRandamValue( 3, 0 ) != 0 ){
			pEnemyManager->AddEnemy( Common::ENEMY_KIND_CCC );
		}
		else{
			pEnemyManager->AddEnemy( Common::ENEMY_KIND_BBB );
		}
	}
	// アイテムの生成
	if( IsCreateItem( m_itemMax, m_itemFrequency ) ){
		uint32_t kind = Utility::GetRandamValue( ItemObject::ITEM_KIND_MAX-1, 0 );
		pItemManager->CreateItem( static_cast<ItemObject::ITEM_KIND>(kind) );
	}
}

/* ================================================ */
/**
 * @brief	クラス初期化
 */
/* ================================================ */
void GameManager::ResetManageValue()
{
	m_type = TYPE_MAX;
	m_gameTimer			= 0;	// ゲームのプレイ時間
	m_gameTimeMax		= 0;	// ゲーム終了までの時間(/fps)
	m_destroyNum		= 0;	// 現在までの敵殲滅数
	m_destroyMax		= 0;	// ステージ終了までの敵殲滅数
	m_enemyMax			= 0;	// 最大出現敵数
	m_enemyFrequency	= 0;	// 敵の出現率(10段階0~9)
	m_itemMax			= 0;	// 最大出現アイテム数
	m_itemFrequency		= 0;	// アイテムの出現率(10段階0~9)
	m_settingFileStr	= "";	// 読み込むステージ設定ファイル
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
	picojson::value stageData = root.get("stageKind");
	std::string typeStr = stageData.get(0).get("type").get<std::string>();
	m_type	= ( typeStr == "DESTROY" ? TYPE_DESTROY : TYPE_TIME );
	m_gameTimeMax	= static_cast<uint32_t>(stageData.get(1).get("TimeS").get<double>());
	m_destroyMax	= static_cast<uint32_t>(stageData.get(2).get("destroyMax").get<double>());
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
			uint32_t rand = static_cast<uint32_t>( Utility::GetRandamValue( 1000, 0 ) );
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
			uint32_t rand = static_cast<uint32_t>( Utility::GetRandamValue( 1000, 0 ) );
			if( rand < ( 5 + (5*frequency) )){
				isCreate = true;	
			}
		}
	}
	return isCreate;
}

