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
#include "GameRecorder.h"
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
			m_settingFileStr = "GameSettings01.json";
			break;
		case Common::FLOW_STAGE02:
			m_settingFileStr = "GameSettings02.json";
			break;
		case Common::FLOW_STAGE03:
			m_settingFileStr = "GameSettings03.json";
			break;
		case Common::FLOW_STAGE04:
			m_settingFileStr = "GameSettings04.json";
			break;
		case Common::FLOW_STAGE05:
			m_settingFileStr = "GameSettings05.json";
			break;
		case Common::FLOW_STAGE06:
			m_settingFileStr = "GameSettings06.json";
			break;
		case Common::FLOW_STAGE07:
			m_settingFileStr = "GameSettings07.json";
			break;
		case Common::FLOW_STAGE08:
			m_settingFileStr = "GameSettings08.json";
			break;
		case Common::FLOW_STAGE09:
			m_settingFileStr = "GameSettings09.json";
			break;
		case Common::FLOW_STAGE10:
			m_settingFileStr = "GameSettings10.json";
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
	// 初期配置に設定されている敵キャラを生成
	for( uint32_t i = 0; i < m_initEnemyInfoVec.size(); ++i ){
		math::Vector2 pos;
		if( m_initEnemyInfoVec.at(i).m_initPos == math::Vector2() ){
			pos = Utility::GetMapRandamPos( /*bool allowInWindow=*/true );
		}
		else{
			pos = math::Vector2( m_initEnemyInfoVec.at(i).m_initPos.x, m_initEnemyInfoVec.at(i).m_initPos.y );
		}
		CreateEnemy( m_initEnemyInfoVec.at(i).m_kind, m_initEnemyInfoVec.at(i).m_level, true, pos );
	}
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
		EnemyManager *pEnemyManager = GameRegister::GetInstance()->UpdateManagerEnemy();
		if( m_destroyNum >= m_destroyMax && pEnemyManager->CountEnemy() == 0 ){
			retVal = true;
		}
	}
	else if( m_type == TYPE_DESTROY_BOSS ){
		EnemyManager *pEnemyManager = GameRegister::GetInstance()->UpdateManagerEnemy();
		if( pEnemyManager ){
			// ボスとして指定された敵がいるかどうか確認
			for( uint32_t i = 0; i < m_bossKindVec.size() ; ++i ){
				if( pEnemyManager->CountEnemy( m_bossKindVec.at(i) ) == 0 ){
					retVal = true;
				}
				else{
					retVal = false;
					break;
				}
			}
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
 * @brief	アイテム生成依頼
 */
/* ================================================ */
void GameManager::CreateItem( const Common::ITEM_KIND &kind, const math::Vector2 &pos )
{
	if( IsCreateItem( m_itemMax ) ){
		ItemManager *pItemManager = GameRegister::GetInstance()->UpdateManagerItem();
		pItemManager->CreateItem( kind, pos );
	}
}

/* ================================================ */
/**
 * @brief	敵生成依頼
 */
/* ================================================ */
void GameManager::CreateEnemy( const Common::ENEMY_KIND &kind, const uint32_t &level, bool isForce, const math::Vector2 &enemyPos )
{
	if( IsCreateEnemy( m_enemyMax ) || isForce ){
		EnemyManager *pEnemyManager = GameRegister::GetInstance()->UpdateManagerEnemy();
		pEnemyManager->AddEnemy( kind, level, enemyPos );
	}
}

/* ================================================ */
/**
 * @brief	毎フレーム処理
 */
/* ================================================ */
void GameManager::Update()
{
	++m_gameTimer;

	// 敵の生成
	if( IsCreateEnemy( m_enemyMax, m_enemyFrequency ) ){
		uint32_t seedRandamValue = Utility::GetRandamValue( 100, 1 );
		uint32_t ratio = 0;
		for( uint32_t i = 0; i < m_enemyInfoVec.size() ; ++i ){
			ratio += m_enemyInfoVec.at(i).m_freequency;
			if( seedRandamValue < ratio ){
				CreateEnemy( m_enemyInfoVec.at(i).m_kind, m_enemyInfoVec.at(i).m_level );
				break;
			}
		}
	}
	// アイテムの生成
	if( IsCreateItem( m_itemMax, m_itemFrequency ) ){
		uint32_t kind = Utility::GetRandamValue( Common::ITEM_KIND_ANTIDOTE-1, 0 );
		CreateItem( static_cast<Common::ITEM_KIND>(kind) );
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
	m_enemyInfoVec.clear();		// 出現する敵情報
}

/* ================================================ */
/**
 * @brief	環境情報取得
 */
/* ================================================ */
void GameManager::LoadGameSettings( const char *jsonFile )
{
	picojson::value null;
	std::string path = JSON_OTHER_PATH;
	path += jsonFile;
	idxfstream ifs(path.c_str());

	picojson::value root;
	picojson::parse( root, ifs);

	//!基本となる情報取得(ステージのクリア条件)
	{
		picojson::value stageData = root.get("stageKind");
		std::string typeStr = stageData.get(0).get("type").get<std::string>();
		if( typeStr.compare("TIME") == 0 ){
			m_type = TYPE_TIME;
			m_gameTimeMax	= static_cast<uint32_t>(stageData.get(1).get("TimeS").get<double>());
			m_gameTimeMax *= 60;
		}
		else if( typeStr.compare("DESTROY") == 0 )
		{
			m_type = TYPE_DESTROY;
			m_destroyMax	= static_cast<uint32_t>(stageData.get(2).get("destroyMax").get<double>());
		}
		else if( typeStr.compare("DESTROY_BOSS") == 0 )
		{
			m_type = TYPE_DESTROY_BOSS;
			picojson::value bossData = root.get("bossInfo");
			picojson::value null;
			for(uint32_t i = 0;; ++i){
				if( bossData == null || bossData.get(i) == null){
					break;
				}
				
				// ボスカウントする敵の種類を取得
				Common::ENEMY_KIND kind = GetEnemyKindFromStr( bossData.get(i).get("enemyKind").get<std::string>() );
				if( kind != Common::ENEMY_KIND_MAX ){
					m_bossKindVec.push_back( kind );
				}

				// 初期配置するボス情報セット
				uint32_t enemyIndex = static_cast<uint32_t>( bossData.get(i).get("num").get<double>() );
				for( uint32_t j = 0; j < enemyIndex; ++j ){
					ExistEnemyState enemyState;
					enemyState.Init();
					enemyState.m_kind = kind;
					enemyState.m_level = static_cast<uint32_t>( bossData.get(i).get("level").get<double>() );
					if( bossData.get(i).get("initPosX") != null && bossData.get(i).get("initPosY") != null ){
						enemyState.m_initPos.x = static_cast<float>( bossData.get(i).get("initPosX").get<double>() );
						enemyState.m_initPos.y = static_cast<float>( bossData.get(i).get("initPosY").get<double>() );
					}
					m_initEnemyInfoVec.push_back( enemyState );
				}
			}
		}
	}

	// 初期配置の敵設定
	{
		picojson::value initEnemy = root.get("initEnemy");
		for( uint32_t i = 0 ;; ++i ){
			if( initEnemy == null || initEnemy.get(i) == null ){
				break;
			}
			uint32_t enemyIndex = static_cast<uint32_t>( initEnemy.get(i).get("num").get<double>() );
			for( uint32_t j = 0; j < enemyIndex ; ++j ){
				ExistEnemyState enemyState;
				enemyState.Init();
				enemyState.m_kind = GetEnemyKindFromStr( initEnemy.get(i).get("enemyKind").get<std::string>() );
				enemyState.m_level = static_cast<uint32_t>( initEnemy.get(i).get("level").get<double>() );
				if( initEnemy.get(i).get("initPosX") != null && initEnemy.get(i).get("initPosY") != null ){
					enemyState.m_initPos.x = static_cast<float>( initEnemy.get(i).get("initPosX").get<double>() );
					enemyState.m_initPos.y = static_cast<float>( initEnemy.get(i).get("initPosY").get<double>() );
				}
				m_initEnemyInfoVec.push_back( enemyState );
			}
		}
	}

	// 出現敵の頻度
	{
		picojson::value enemyData = root.get("appearEnemy");
		uint32_t totalFreequency = 0;	// 出現係数のトータル
		for( uint32_t i = 0 ;; ++i ){
			if( enemyData == null || enemyData.get(i) == null ){
				break;
			}
			ExistEnemyState eneInfo;
			eneInfo.m_kind = GetEnemyKindFromStr( enemyData.get(i).get("enemyKind").get<std::string>() );
			eneInfo.m_level = static_cast<uint32_t>(enemyData.get(i).get("level").get<double>());
			eneInfo.m_freequency = static_cast<uint32_t>(enemyData.get(i).get("frequency").get<double>());
			if( eneInfo.m_level == 0 ){
				DEBUG_ASSERT( 0, "eneInfo.m_level is 0" );
				eneInfo.m_level = 1;
			}
			m_enemyInfoVec.push_back( eneInfo );

			totalFreequency += eneInfo.m_freequency;
		}
		// 出現係数の合計からそれぞれの敵の出現率を求めてセットしなおす
		for( uint32_t i = 0; i < m_enemyInfoVec.size() ; ++i ){
			float ratio =  m_enemyInfoVec.at(i).m_freequency / static_cast<float>( totalFreequency );
			m_enemyInfoVec.at(i).m_freequency = static_cast<uint32_t>( (ratio * 100.0f) + 0.5f );
		}
	}

	// 敵、アイテムの出現頻度
	{
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

	// 取得できるバトルポイント情報
	{
		picojson::value pointData = root.get("pointInfo");
		for( uint32_t i = 0 ;; ++i ){
			if( pointData == null || pointData.get(i) == null ){
				break;
			}
			uint32_t score = static_cast<uint32_t>(pointData.get(i).get("score").get<double>());
			uint32_t point = static_cast<uint32_t>(pointData.get(i).get("value").get<double>());
			GameRecorder::GetInstance()->SetClearBattlePoint( score, point );
		}
	}
}

/* ================================================ */
/**
 * @brief	敵の生成判断
 */
/* ================================================ */
bool GameManager::IsCreateEnemy( uint32_t enemyLimit, uint32_t frequency )
{
	bool isCreate = false;
	EnemyManager *pEnemyManager = GameRegister::GetInstance()->UpdateManagerEnemy();	
	// 敵の生成
	if( pEnemyManager ){
		uint32_t currEnemy = pEnemyManager->CountEnemy();
		uint32_t under = frequency + 1;
		if( currEnemy < (enemyLimit / under) || currEnemy == 0 ){
			// 現在の出現敵数が一定の割合以下なら敵生成
			isCreate = true;
		}
		else if( currEnemy < enemyLimit ){
			// 出現頻度によって生成
			uint32_t rand = static_cast<uint32_t>( Utility::GetRandamValue( 1000, 0 ) );
			if( rand < ( 5 + (5*frequency) )){
				isCreate = true;	
			}
		}

		if( m_type == TYPE_DESTROY ){
			if( m_destroyNum + currEnemy >= m_destroyMax ){
				// 既に敵がステージクリア条件の撃破数分生成済みなのでこれ以上出現させない
				isCreate = false;
			}
		}
	}
	return isCreate;
}
bool GameManager::IsCreateEnemy( uint32_t enemyLimit )
{
	bool isCreate = false;
	EnemyManager *pEnemyManager = GameRegister::GetInstance()->UpdateManagerEnemy();	
	// 敵の生成
	if( pEnemyManager ){
		uint32_t currEnemy = pEnemyManager->CountEnemy();
		if( currEnemy < enemyLimit ){
			isCreate = true;
		}
	}
	return isCreate;
}

/* ================================================ */
/**
 * @brief	アイテムの生成判断
 */
/* ================================================ */
bool GameManager::IsCreateItem( uint32_t itemLimit, uint32_t frequency )
{
	bool isCreate = false;
	ItemManager *pEnemyManager = GameRegister::GetInstance()->UpdateManagerItem();	
	// アイテムの生成
	if( pEnemyManager ){
		uint32_t currItem = pEnemyManager->CountItem();
		if( currItem <= itemLimit ){
			// 出現頻度によって生成
			uint32_t rand = static_cast<uint32_t>( Utility::GetRandamValue( 1000, 1 ) );
			if( rand < ( 5*frequency )){
				isCreate = true;	
			}
		}
	}
	return isCreate;
}
bool GameManager::IsCreateItem( uint32_t itemLimit )
{
	bool isCreate = false;
	ItemManager *pEnemyManager = GameRegister::GetInstance()->UpdateManagerItem();	
	// アイテムの生成
	if( pEnemyManager ){
		uint32_t currItem = pEnemyManager->CountItem();
		if( currItem <= itemLimit ){
			isCreate = true;
		}
	}
	return isCreate;
}


/* ================================================ */
/**
 * @brief	敵の種類を文字列から取得
 */
/* ================================================ */
Common::ENEMY_KIND GameManager::GetEnemyKindFromStr( const std::string str )
{
	static struct enemyKindPair{
		Common::ENEMY_KIND	kind;
		std::string			kindStr;
	} s_enemyTypeStr[] = {
		{ Common::ENEMY_KIND_SLIME,			"ENEMY_KIND_SLIME" },
		{ Common::ENEMY_KIND_SLIME_ANOTHER,	"ENEMY_KIND_SLIME_ANOTHER" },
		{ Common::ENEMY_KIND_AHRIMAN,		"ENEMY_KIND_AHRIMAN" },
		{ Common::ENEMY_KIND_COW,			"ENEMY_KIND_COW" },
		{ Common::ENEMY_KIND_RANGER,		"ENEMY_KIND_RANGER" },
		{ Common::ENEMY_KIND_SLIME_KING,	"ENEMY_KIND_SLIME_KING" },
		{ Common::ENEMY_KIND_WIZARD,		"ENEMY_KIND_WIZARD" },
		{ Common::ENEMY_KIND_DRAGON,		"ENEMY_KIND_DRAGON" },
		{ Common::ENEMY_KIND_LAST_BOSS,		"ENEMY_KIND_LAST_BOSS" },
	};
	for( uint32_t i = 0; i < NUMBEROF(s_enemyTypeStr); ++i ){
		if( s_enemyTypeStr[i].kindStr.compare( str ) == 0 ){
			return s_enemyTypeStr[i].kind;
		}
	}
	DEBUG_ASSERT( 0, "jsonから読み込んだ敵のタイプが想定外");
	return Common::ENEMY_KIND_MAX;
}

