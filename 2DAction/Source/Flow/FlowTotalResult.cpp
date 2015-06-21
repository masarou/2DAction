/* ====================================================================== */
/**
 * @brief  
 *		ゲーム結果画面クラス
 * @note
 *		各ステージのスコアを表示して、合算を出す画面
 */
/* ====================================================================== */

#include <fstream>
#include <algorithm>
#include "FlowTotalResult.h"
#include "Game/GameRecorder.h"
#include "Common/Utility/CommonGameUtility.h"
#include "System/Draw2D/SystemDraw2DResource.h"

FlowBase *FlowTotalResult::Create( const std::string &fileName )
{
	return NEW FlowTotalResult(fileName);
}

FlowTotalResult::FlowTotalResult( const std::string &fileName )
: FlowMenuBase(fileName)
{
	DEBUG_PRINT("FlowTotalResult生成！！\n");
}


FlowTotalResult::~FlowTotalResult(void)
{
	DEBUG_PRINT("FlowTotalResult削除！！\n");
}

bool FlowTotalResult::Init()
{
	// 背景一枚絵作成
	m_pMenuWindow = TotalResult2D::CreateTotalResult2D( "MenuGameResult.json" );
	return true;
}

bool FlowTotalResult::FinishFlow()
{
	// 今回のゲームの結果反映
	RecordGameResult();

	// 常駐物で初期化が必要なものリセット
	Utility::GameInitALL();
	return true;
}

void FlowTotalResult::RecordGameResult()
{
	// 今回のプレイを反映させたランキング作成
	Common::SAVE_DATA updateData;
	bool retVal = Utility::GetSaveData( updateData );
	if( retVal ){
		// ランキング作成
		UpdateSortRanking( updateData );

		// 取得したバトルポイントを反映させる
		updateData.m_battlePoint = 10*GameRecorder::GetInstance()->GetItemCount( Common::ITEM_KIND_BATTLE_POINT );
	}
	
	// セーブデータ上書き
	Utility::OverWriteSaveData( updateData );
}


bool sortScore(const uint32_t &left, const uint32_t &rRight)
{
	return left > rRight;
}

void FlowTotalResult::UpdateSortRanking( Common::SAVE_DATA &scoreData )
{
	// 今までのスコアをpush
	std::vector<uint32_t> ranking;
	for( uint32_t i = 0; i < NUMBEROF(scoreData.m_scoreRanking) ; ++i ){
		ranking.push_back(scoreData.m_scoreRanking[i]);
	}

	// 今回のスコアをpush
	uint32_t scoreCurr = GameRecorder::GetInstance()->GetTotalScore();
	ranking.push_back(scoreCurr);

	// 並び替え
	std::sort( ranking.begin(), ranking.end(), sortScore);
	
	for( uint32_t i = 0; i < NUMBEROF(scoreData.m_scoreRanking) ; ++i ){
		scoreData.m_scoreRanking[i] = ranking.at(i);
	}
}

/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		表示するスコアそのもの
 */
/* ====================================================================== */
TotalResult2D *TotalResult2D::CreateTotalResult2D( const std::string &readMenuJson )
{
	return NEW TotalResult2D( readMenuJson );
}

TotalResult2D::TotalResult2D( const std::string &readMenuJson )
: MenuWindow( readMenuJson )
, m_dispState(DISP_STAGE01)
{
	for( uint32_t i = 0; i < DISP_MAX ; ++i ){
		m_pNumCounter[i] = NULL;
	}
}

TotalResult2D::~TotalResult2D(void)
{
}

bool TotalResult2D::InitMenu()
{
	static const char *s_counterName[DISP_MAX] = {
		"scoreStage01",
		"scoreStage02",
		"scoreStage03",
		"scoreTotal",
	};

	// 数字カウンタセット
	for( uint32_t i = 0; i < NUMBEROF(s_counterName) ; ++i ){
		m_pNumCounter[i] = GetPartsCounter( s_counterName[i] );
		if( m_pNumCounter[i] == NULL ){
			DEBUG_ASSERT( 0, "カウンタパーツが見つからない");
			return true;
		}
	}

	// 敵を倒して得た得点をセット
	m_pNumCounter[static_cast<uint32_t>(DISP_STAGE01)]->AddValue( GameRecorder::GetInstance()->GetStageScore( GameRecorder::STATE_STAGE01 ) );

	return true;
}

void TotalResult2D::UpdateMenu()
{
	CallPadEvent();

	// アニメカウントが終わっているなら次のステップに進む
	uint32_t index = static_cast<uint32_t>( m_dispState );
	switch(m_dispState){
	case DISP_STAGE01:
		if( !m_pNumCounter[index]->IsPlayCountAnim() ){
			m_dispState = DISP_STAGE02;
			m_pNumCounter[static_cast<uint32_t>(DISP_STAGE02)]->AddValue( GameRecorder::GetInstance()->GetStageScore( GameRecorder::STATE_STAGE02 ) );
		}
		break;
	case DISP_STAGE02:
		if( !m_pNumCounter[index]->IsPlayCountAnim() ){
			m_dispState = DISP_STAGE03;
			m_pNumCounter[static_cast<uint32_t>(DISP_STAGE03)]->AddValue( GameRecorder::GetInstance()->GetStageScore( GameRecorder::STATE_STAGE03 ) );
		}
		break;
	case DISP_STAGE03:
		if( !m_pNumCounter[index]->IsPlayCountAnim() ){
			m_dispState = DISP_TOTAL;
			m_pNumCounter[static_cast<uint32_t>(DISP_TOTAL)]->AddValue( GameRecorder::GetInstance()->GetTotalScore() );
		}
		break;
	case DISP_TOTAL:
		if( !m_pNumCounter[index]->IsPlayCountAnim() ){
			m_dispState = DISP_MAX;
		}
		break;
	case DISP_MAX:

		break;
	}

}

void TotalResult2D::PadEventDecide()
{
	uint32_t index = static_cast<uint32_t>( m_dispState );
	switch(m_dispState){
	case DISP_STAGE01:
		// カウントアニメ終了
		m_pNumCounter[index]->CountAnimEnd();
		m_dispState = DISP_STAGE02;

		m_pNumCounter[static_cast<uint32_t>(DISP_STAGE02)]->AddValue( GameRecorder::GetInstance()->GetStageScore( GameRecorder::STATE_STAGE02 ) );
		break;
	case DISP_STAGE02:
		// カウントアニメ終了
		m_pNumCounter[index]->CountAnimEnd();
		m_dispState = DISP_STAGE03;

		m_pNumCounter[static_cast<uint32_t>(DISP_STAGE03)]->AddValue( GameRecorder::GetInstance()->GetStageScore( GameRecorder::STATE_STAGE03 ) );
		break;
	case DISP_STAGE03:
		// カウントアニメ終了
		m_pNumCounter[index]->CountAnimEnd();
		m_dispState = DISP_TOTAL;

		m_pNumCounter[static_cast<uint32_t>(DISP_TOTAL)]->AddValue( GameRecorder::GetInstance()->GetTotalScore() );
		break;
	case DISP_TOTAL:
		// カウントアニメ終了
		m_pNumCounter[index]->CountAnimEnd();
		m_dispState = DISP_MAX;
		break;
	case DISP_MAX:
		SetNextFlowStr( "title" );
		break;
	}
}
