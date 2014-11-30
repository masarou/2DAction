
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

FlowBase *FlowGame::Create( const std::string &fileName )
{
	return NEW FlowGame(fileName);
}

FlowGame::FlowGame( const std::string &fileName )
	: FlowBase(fileName)
	, m_gameTimer( 0 )
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

	return true;
}

void FlowGame::UpdateFlow()
{
	FlowBase::UpdateFlow();

	++m_gameTimer;

	if( GameRegister::GetInstance()->GetPlayer()->GetCurrentLife() == 0 ){
		StartFade("gameend");
	}
	else if( m_gameTimer > 6000 ){
		StartFade("gameend");
	}
}