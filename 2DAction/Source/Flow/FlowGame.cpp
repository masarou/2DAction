
/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		タイトル画面管理クラス
 */
/* ====================================================================== */

#include "FlowGame.h"
#include "Game/GameRegister.h"

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
	// ゲーム周りのクラス解放
	GameRegister *pRegister = GameRegister::GetInstance();
	SAFE_DELETE( pRegister );

	DEBUG_PRINT("FlowGame削除！！\n");
}

bool FlowGame::Init()
{
	// ゲームをするのに必要なインスタンス作成
	GameRegister::CreateInstance();

	return true;
}

void FlowGame::FlowUpdate()
{
	ChildUpdate();

	//if(player->IsNext()){
	//	StartFade("proceed");
	//}
}