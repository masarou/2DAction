
/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		�Q�[����ʊǗ��N���X
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
	DEBUG_PRINT("FlowGame�����I�I\n");
}


FlowGame::~FlowGame(void)
{
	// �Q�[������̃N���X���
	GameRegister::GetInstance()->DeleteRegister();

	DEBUG_PRINT("FlowGame�폜�I�I\n");
}

bool FlowGame::Init()
{
	// �Q�[��������̂ɕK�v�ȃC���X�^���X�쐬
	GameRegister::CreateInstance();

	// �}�b�v�̊��擾
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

	//!��{�ƂȂ���擾
	picojson::value timeData = root.get("gameTime");
	m_gameTimeMax	= static_cast<uint32_t>(timeData.get(0).get("TimeS").get<double>());
	m_gameTimeMax *= 60;
	
	picojson::value condData = root.get("gameCondition");
	m_enemyMax			= static_cast<uint32_t>(condData.get(0).get("EnemyMax").get<double>());
	m_enemyFrequency	= static_cast<uint32_t>(condData.get(0).get("frequency").get<double>());
	m_itemMax			= static_cast<uint32_t>(condData.get(1).get("ItemMax").get<double>());
	m_itemFrequency		= static_cast<uint32_t>(condData.get(1).get("frequency").get<double>());
}

