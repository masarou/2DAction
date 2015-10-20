/* ====================================================================== */
/**
 * @brief  
 *		ゲームステージ結果画面クラス
 * @note
 *		
 */
/* ====================================================================== */

#include <fstream>
#include <algorithm>
#include "FlowStageResult.h"
#include "FlowManager.h"
#include "Game/GameRecorder.h"
#include "Common/Utility/CommonGameUtility.h"
#include "System/Draw2D/SystemDraw2DResource.h"

FlowBase *FlowStageResult::Create( const std::string &fileName )
{
	return NEW FlowStageResult(fileName);
}

FlowStageResult::FlowStageResult( const std::string &fileName )
: FlowMenuBase( fileName )
{
	DEBUG_PRINT("FlowStageResult生成！！\n");
}


FlowStageResult::~FlowStageResult(void)
{
	DEBUG_PRINT("FlowStageResult削除！！\n");
}

bool FlowStageResult::Init()
{
	// 背景一枚絵作成
	if( GameRecorder::GetInstance()->IsUserClear() ){
		m_pMenuWindow = ResultStageMenu::CreateResultStageMenu( "MenuStageResult.json" );
	}
	else{
		m_pMenuWindow = ResultStageMenu::CreateResultStageMenu( "MenuStageResultGameOver.json" );
	}
	return true;
}

void FlowStageResult::UpdateFlowAfterChildTask()
{

}

/* ====================================================================== */
/**
 * @brief  
 *			ステージ結果の中身カウンタクラス
 * @note
 *		
 */
/* ====================================================================== */
ResultStageMenu *ResultStageMenu::CreateResultStageMenu( const std::string &readMenuJson )
{
	return NEW ResultStageMenu( readMenuJson );
}

ResultStageMenu::ResultStageMenu(  const std::string &readMenuJson  )
: MenuWindow( readMenuJson )
, m_dispState(DISP_RESULT)
{
}

ResultStageMenu::~ResultStageMenu(void)
{
}

bool ResultStageMenu::InitMenu()
{
	if( FlowManager::GetInstance()->GetPreFlowKind() != Common::FLOW_STAGE10 ){
		// BGM再生開始
		SoundManager::GetInstance()->PlayBGM( "interval" );
	}

	// 敵を倒して得た得点をセット
	PartsCounter *pCounter = GetPartsCounter( "result" );
	if( pCounter ){
		pCounter->AddValue( GameRecorder::GetInstance()->GetScore() );
	}

	return true;
}

void ResultStageMenu::UpdateMenu()
{
	CallPadEvent();

	// アニメカウントが終わっているなら次のステップに進む
	PartsCounter *pCounter = NULL;
	switch(m_dispState){
	case DISP_RESULT:
		{
			pCounter = GetPartsCounter( "result" );
		}
		break;
	case DISP_BONUS:
		{
			pCounter = GetPartsCounter( "bonus" );
		}
		break;
	case DISP_TOTAL:
		{
			pCounter = GetPartsCounter( "total" );
		}
		break;
	case DISP_BATTLEPOINT:
		
		{
			pCounter = GetPartsCounter( "battlepoint" );
		}
		break;
	case DISP_ALL:

		break;
	}

	if( pCounter && !pCounter->IsPlayCountAnim() ){
		ChangeDispStateNext();
	}
}

void ResultStageMenu::PadEventDecide()
{
	if( ChangeDispStateNext() ){
		// 次に進む
		GameRecorder *pRecorder = GameRecorder::GetInstance();
		if( !pRecorder ){
			DEBUG_ASSERT( 0, "GameRecorder is NULL");
			return;
		}
		
		// プレイヤーライフが0になったのでゲームオーバー
		if( !GameRecorder::GetInstance()->IsUserClear() ){
			SetNextFlowStr( "totalresult" );
			return;
		}

		// ステージ3,6をクリアしたらクリアフラグを立てる
		Common::SAVE_DATA saveData;
		Utility::GetSaveData( saveData );

		// 次のステージor画面へ
		switch( FlowManager::GetInstance()->GetPreFlowKind() ){
		default:
			DEBUG_ASSERT( 0, "想定外のフロー" );
			// とりあえずタイトルへ
			SetNextFlowStr( "title" );
			break;
		case Common::FLOW_STAGE01:
			SetNextFlowStr( "nextgame02" );
			break;
		case Common::FLOW_STAGE02:
			SetNextFlowStr( "nextgame03" );
			break;
		case Common::FLOW_STAGE03:
			SetNextFlowStr( "nextgame04" );
			saveData.m_isClearPhase01 = true;
			break;
		case Common::FLOW_STAGE04:
			SetNextFlowStr( "nextgame05" );
			break;
		case Common::FLOW_STAGE05:
			SetNextFlowStr( "nextgame06" );
			break;
		case Common::FLOW_STAGE06:
			SetNextFlowStr( "nextgame07" );
			saveData.m_isClearPhase02 = true;
			break;
		case Common::FLOW_STAGE07:
			SetNextFlowStr( "nextgame08" );
			break;
		case Common::FLOW_STAGE08:
			SetNextFlowStr( "nextgame09" );
			break;
		case Common::FLOW_STAGE09:
			SetNextFlowStr( "nextgame10" );
			break;
		case Common::FLOW_STAGE10:
			SetNextFlowStr( "totalresult" );	// すべてのステージ終了
			break;
		}

		
		Utility::OverWriteSaveData( saveData );

		// 決定SE鳴らす
		SoundManager::GetInstance()->PlaySE("Decide");
	}
}

uint32_t ResultStageMenu::GetStageClearBonus() const
{
	// ボーナス算出方法
	// コンボ数×ユーザーの残りライフ = Bonus

	uint32_t retVal = 0;
	uint32_t stageComboMax = GameRecorder::GetInstance()->GetMaxComboNumOfStage();

	enum SCORE_STAGE {
		SCORE_01 = 100,
		SCORE_02 = 250,
		SCORE_03 = 400,
		SCORE_04 = 600,
		SCORE_05 = 1000,
	};
	SCORE_STAGE scoreStage = SCORE_01;
	for( uint32_t i = 0; i < stageComboMax ; ++i ){
		switch( scoreStage ){
		case SCORE_01:
			retVal += SCORE_01;
			if( i > 20 ){
				scoreStage = SCORE_02;
			}
			break;
		case SCORE_02:
			retVal += SCORE_02;
			if( i > 50 ){
				scoreStage = SCORE_03;
			}
			break;
		case SCORE_03:
			retVal += SCORE_03;
			if( i > 80 ){
				scoreStage = SCORE_04;
			}
			break;
		case SCORE_04:
			retVal += SCORE_04;
			if( i > 100 ){
				scoreStage = SCORE_05;
			}
			break;
		case SCORE_05:
			retVal += SCORE_05;
			break;
		}
	}
	// 残スコアをかける
	retVal = static_cast<uint32_t>( retVal*GameRecorder::GetInstance()->GetUserLifeRatio() );
	return retVal;
}

bool ResultStageMenu::ChangeDispStateNext()
{
	switch(m_dispState){
	case DISP_RESULT:
		{
			// カウントアニメ終了
			PartsCounter *pCounterResult = GetPartsCounter( "result" );
			if( pCounterResult ){
				pCounterResult->CountAnimEnd();
			}

			PartsCounter *pCounterBonus = GetPartsCounter( "bonus" );
			if( pCounterBonus ){
				pCounterBonus->AddValue( GetStageClearBonus() );
				m_dispState = DISP_BONUS;
			}
			else{
				DEBUG_ASSERT( 0, "目的のパーツがない！" );
			}
		}
		break;
	case DISP_BONUS:
		{
			// カウントアニメ終了
			PartsCounter *pCounterResult = GetPartsCounter( "result" );
			PartsCounter *pCounterBonus = GetPartsCounter( "bonus" );
			if( pCounterBonus ){
				pCounterBonus->CountAnimEnd();
			}

			uint32_t totalValue = 0;
			if( pCounterResult && pCounterBonus ){
				totalValue = pCounterResult->GetValue() + pCounterBonus->GetValue();
			}

			PartsCounter *pCounterTotal = GetPartsCounter( "total" );
			if( pCounterTotal ){
				pCounterTotal->AddValue( totalValue );
				m_dispState = DISP_TOTAL;
			}
			else{
				DEBUG_ASSERT( 0, "目的のパーツがない！" );
			}
		}
		break;
	case DISP_TOTAL:
		{
			// カウントアニメ終了
			PartsCounter *pCounterTotal = GetPartsCounter( "total" );
			if( pCounterTotal ){
				pCounterTotal->CountAnimEnd();
			}

			PartsCounter *pCounterBP = GetPartsCounter( "battlepoint" );
			if( pCounterBP ){
				pCounterBP->AddValue( GameRecorder::GetInstance()->GetClearBattlePoint( pCounterTotal->GetValue() ) );
			}

			// battlePointの表示へ
			m_dispState = DISP_BATTLEPOINT;
		}
		break;
	case DISP_BATTLEPOINT:
		{
			// カウントアニメ終了
			PartsCounter *pCounterBP = GetPartsCounter( "battlepoint" );
			if( pCounterBP ){
				pCounterBP->CountAnimEnd();
			}

			// ここで求めたボーナスをステージスコアに加算
			PartsCounter *pCounterBonus = GetPartsCounter( "bonus" );
			if( pCounterBonus ){
				GameRecorder::GetInstance()->AddScoreBonus( pCounterBonus->GetValue() );
			}
			m_dispState = DISP_ALL;
		}
		break;
	case DISP_ALL:
		return true;
		break;
	}
	return false;
}