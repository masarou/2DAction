
/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		�^�C�g����ʊǗ��N���X
 */
/* ====================================================================== */

#include "FlowGame.h"
#include "Game/Player/GamePlayer.h"
#include "Game/GameMap.h"
#include "Game/Enemy/EnemyManager.h"

GamePlayer *player = NULL;

FlowBase *FlowGame::Create(std::string fileName)
{
	return NEW FlowGame(fileName);
}

FlowGame::FlowGame(std::string fileName)
	: FlowBase(fileName)
{
	DEBUG_PRINT("FlowGame�����I�I\n");
}


FlowGame::~FlowGame(void)
{
	DEBUG_PRINT("FlowGame�폜�I�I\n");
}

bool FlowGame::Init()
{
	// ��
	GameMap *map = NEW GameMap();
	player = NEW GamePlayer();
	
	return true;
}

void FlowGame::FlowUpdate()
{
	if(0){
		EnemyManager::GetInstance()->CreateEnemy( Common::KIND_AAA );
	}

	ChildUpdate();

	//if(player->IsNext()){
	//	StartFade("proceed");
	//}
}