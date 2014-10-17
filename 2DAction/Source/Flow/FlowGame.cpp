
/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		タイトル画面管理クラス
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
	DEBUG_PRINT("FlowGame生成！！\n");
}


FlowGame::~FlowGame(void)
{
	DEBUG_PRINT("FlowGame削除！！\n");
}

bool FlowGame::Init()
{
	// 仮
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