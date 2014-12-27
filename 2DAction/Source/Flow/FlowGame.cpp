
/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		ゲーム画面管理クラス
 */
/* ====================================================================== */

#include "FlowGame.h"
#include "Game/GameRegister.h"
#include "System/picojson.h"

FlowBase *FlowGame::Create( const std::string &fileName )
{
	return NEW FlowGame(fileName);
}

FlowGame::FlowGame( const std::string &fileName )
: FlowBase(fileName)
, m_gameTimer( 0 )
, m_gameTimeMax( 0 )
, m_enemyMax( 0 )
, m_enemyFrequency( 0 )
, m_itemMax( 0 )
, m_itemFrequency( 0 )
{
	DEBUG_PRINT("FlowGame生成！！\n");
}


FlowGame::~FlowGame(void)
{
	// ゲーム周りのクラス解放
	GameRegister::GetInstance()->DeleteRegister();

	DEBUG_PRINT("FlowGame削除！！\n");
}

bool FlowGame::Init()
{
	// ゲームをするのに必要なインスタンス作成
	GameRegister::CreateInstance();

	// マップの環境取得
	LoadGameSettings("gameSettings.json");

	return true;
}

void FlowGame::UpdateFlow()
{
	FlowBase::UpdateFlow();

	++m_gameTimer;

	const GamePlayer *pPlayer = GameRegister::GetInstance()->GetPlayer();
	if( pPlayer->GetCurrentLife() == 0 || m_gameTimer >= m_gameTimeMax ){
		StartFade("gameend");
	}
}

void FlowGame::LoadGameSettings( const char *jsonFile )
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
}

