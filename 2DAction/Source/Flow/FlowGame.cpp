
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
#include "Game/GameRecorder.h"

FlowBase *FlowGame::Create( const std::string &fileName )
{
	return NEW FlowGame(fileName);
}

FlowGame::FlowGame( const std::string &fileName )
: FlowBase(fileName)
, m_pNumCounter( NULL )
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
	// ゲーム中に表示するスコア準備
	m_pNumCounter = NumberCounter::Create("number.json");

	// ゲームをするのに必要なインスタンス作成
	GameRegister::CreateInstance();

	return true;
}

void FlowGame::UpdateFlowAfterChildTask()
{
	// 画面内スコア表示の更新
	if( m_pNumCounter ){
		uint32_t currScore = GameRecorder::GetInstance()->GetScore();
		m_pNumCounter->SetValue( currScore );
	}

	const GamePlayer *pPlayer = GameRegister::GetInstance()->GetPlayer();
	const GameManager *pGameMan = GameRegister::GetInstance()->GetManagerGame();
	if( pPlayer->GetCurrentLife() == 0 /*|| pGameMan->IsGameOver()*/ ){
		StartFade("gameend");
	}
}
