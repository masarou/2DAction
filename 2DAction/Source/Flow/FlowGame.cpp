/* ====================================================================== */
/**
 * @brief  
 *		ゲーム画面総括クラス
 * @note
 *		
 */
/* ====================================================================== */
#include "FlowGame.h"
#include "Game/GameRegister.h"
#include "Game/GameRecorder.h"
#include "Game/Timer/GameStageTimer.h"
#include "Flow/Process/FlowProcessFirstManual.h"
#include "Common/Utility/CommonGameUtility.h"
#include "Process/FlowProcessStageStart.h"

FlowBase *FlowGame::Create( const std::string &fileName )
{
	return NEW FlowGame(fileName);
}

FlowGame::FlowGame( const std::string &fileName )
: FlowBase(fileName)
, m_pNumScore( NULL )
, m_pStageTimer( NULL )
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

	const GameManager *pGameManager = GameRegister::GetInstance()->GetManagerGame();
	if( pGameManager->GetStageType() == GameManager::TYPE_TIME ){
		// 残り時間を表示するクラス用意
		m_pStageTimer = StageTimer::CreateStageTimer( pGameManager->GetGameLeftTimeBySec() );
	}

	std::string soundTag = "";
	std::string readFileScore = "Number.json";
	switch( FlowManager::GetInstance()->GetCurrentFlowKind() ){
	case Common::FLOW_STAGE01:
	case Common::FLOW_STAGE02:
		{
			soundTag = "stage01";
		}
		break;
	case Common::FLOW_STAGE03:
		soundTag = "BossBattle";
		break;

	case Common::FLOW_STAGE04:
	case Common::FLOW_STAGE05:
		soundTag = "stage02";
		break;

	case Common::FLOW_STAGE06:
		soundTag = "BossBattle";
		break;

	case Common::FLOW_STAGE07:
		soundTag = "stage03";
		readFileScore = "NumberWhite.json";	// 背景が黒なので見やすい白文字でScore表示
		break;

	case Common::FLOW_STAGE08:
		soundTag = "stage03";
		readFileScore = "NumberWhite.json";
		break;

	case Common::FLOW_STAGE09:
		soundTag = "BossBattle";
		readFileScore = "NumberWhite.json";
		break;

	case Common::FLOW_STAGE10:
		soundTag = "stageLast";
		readFileScore = "NumberWhite.json";
		break;
	}

	// ゲーム中に表示するスコア準備
	m_pNumScore = NumberCounter::Create( readFileScore.c_str() );
	if( m_pNumScore ){
		// 数値が変化するときにSEは鳴らさない
		m_pNumScore->SetCountUpSeInvalidFlag( true );
	}

	// BGM再生開始
	SoundManager::GetInstance()->PlayBGM( soundTag.c_str() );


	// 初めてのゲームプレイかどうかチェック
	Common::SAVE_DATA saveData;
	Utility::GetSaveData( saveData );
	if( saveData.m_isFirstGamePlay ){
		// 操作説明セット
		FirstManual *pFirstManual = FirstManual::Create( FirstManual::KIND_GAMEPLAY01 );
		PushStageEffect( pFirstManual );
		pFirstManual = FirstManual::Create( FirstManual::KIND_GAMEPLAY02 );
		PushStageEffect( pFirstManual );

		// 初めてのゲームプレイフラグを下げる
		saveData.m_isFirstGamePlay = false;
		Utility::OverWriteSaveData( saveData );
	}

	// いくつめのステージかを画面に出す
	StageStart *pStageStart = StageStart::Create();
	PushStageEffect( pStageStart );

	return true;
}

void FlowGame::UpdateFlowAfterChildTask()
{
	// 画面内スコア表示の更新
	if( m_pNumScore ){
		uint32_t currScore = GameRecorder::GetInstance()->GetScore();
		m_pNumScore->SetValue( currScore );
	}

	const GamePlayer *pPlayer = GameRegister::GetInstance()->GetPlayer();
	const GameManager *pGameMan = GameRegister::GetInstance()->GetManagerGame();
	if( ( pPlayer->GetCurrentLife() == 0 || pGameMan->IsGameOver() )
		&& !IsPlaySpecialEffect() ){

		StartFade("gameend");

		// 終了時点でのユーザーライフを格納しておく
		const GamePlayer *pPlayer = GameRegister::GetInstance()->GetPlayer();
		uint32_t leftLife = pPlayer->GetCurrentLife();
		float ratio = static_cast<float>(leftLife) / static_cast<float>(pPlayer->GetPlayerLifeMax());
		GameRecorder::GetInstance()->SetUserLifeRatio( ratio );
	}
}
