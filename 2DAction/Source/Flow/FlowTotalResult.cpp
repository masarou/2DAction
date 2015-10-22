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
	// BGM再生開始
	SoundManager::GetInstance()->PlayBGM( "interval" );

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

/* ====================================================================== */
/**
 * @brief セーブデータの更新
 */
/* ====================================================================== */
void FlowTotalResult::RecordGameResult()
{
	// 今回のプレイを反映させたランキング作成
	Common::SAVE_DATA updateData;
	bool retVal = Utility::GetSaveData( updateData );
	if( retVal ){

		// 最初からプレイしたかどうかチェック
		//if( !GameRecorder::GetInstance()->IsContinue() ){
			// ランキング作成
			UpdateSortRanking( updateData );
		//}

		// 取得したバトルポイントを反映させる
		updateData.m_battlePoint = GameRecorder::GetInstance()->GetClearBattlePointAll();
	}
	
	// セーブデータ上書き
	Utility::OverWriteSaveData( updateData );
}

/* ====================================================================== */
/**
 * @brief 引数のランキングをソート
 */
/* ====================================================================== */
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
, m_dispState(DISP_SCENE01)
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
		"scoreScene01",
		"scoreScene02",
		"scoreScene03",
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
	uint32_t sceneScore = GameRecorder::GetInstance()->GetStageScore( GameRecorder::STATE_STAGE01 )
						+ GameRecorder::GetInstance()->GetStageScore( GameRecorder::STATE_STAGE02 )
						+ GameRecorder::GetInstance()->GetStageScore( GameRecorder::STATE_STAGE03 );
	m_pNumCounter[static_cast<uint32_t>(DISP_SCENE01)]->AddValue( sceneScore );

	return true;
}

void TotalResult2D::UpdateMenu()
{
	CallPadEvent();

	// アニメカウントが終わっているなら次のステップに進む
	PartsCounter *pCounter = NULL;
	uint32_t index = static_cast<uint32_t>( m_dispState );
	switch(m_dispState){
	case DISP_SCENE01:
	case DISP_SCENE02:
	case DISP_SCENE03:
	case DISP_TOTAL:
		pCounter = m_pNumCounter[index];
		break;
	case DISP_MAX:

		break;
	}

	if( pCounter && !pCounter->IsPlayCountAnim() ){
		ChangeDispNextState();
	}

}

void TotalResult2D::PadEventDecide()
{
	if( ChangeDispNextState() ){

		if( GameRecorder::GetInstance()->IsClearAll() ){
			// 全部クリアーならアドレス表示画面へ
			SetNextFlowStr( "gameaddress" );
		}
		else{
			SetNextFlowStr( "title" );
		}
		// 決定SE鳴らす
		SoundManager::GetInstance()->PlaySE("Decide");
	}
}

/* ====================================================================== */
/**
 * @brief 画面の表示更新項目を次のものへ
 */
/* ====================================================================== */
bool TotalResult2D::ChangeDispNextState()
{
	if( m_dispState == DISP_MAX ){
		return true;
	}

	uint32_t index = static_cast<uint32_t>( m_dispState );
	m_pNumCounter[index]->CountAnimEnd();
	switch(m_dispState){
	case DISP_SCENE01:
		{
			m_dispState = DISP_SCENE02;
			uint32_t sceneScore = GameRecorder::GetInstance()->GetStageScore( GameRecorder::STATE_STAGE04 )
								+ GameRecorder::GetInstance()->GetStageScore( GameRecorder::STATE_STAGE05 )
								+ GameRecorder::GetInstance()->GetStageScore( GameRecorder::STATE_STAGE06 );
			m_pNumCounter[static_cast<uint32_t>(DISP_SCENE02)]->AddValue( sceneScore );
		}
		break;
	case DISP_SCENE02:
		{
			m_dispState = DISP_SCENE03;		
			uint32_t sceneScore = GameRecorder::GetInstance()->GetStageScore( GameRecorder::STATE_STAGE07 )
								+ GameRecorder::GetInstance()->GetStageScore( GameRecorder::STATE_STAGE08 )
								+ GameRecorder::GetInstance()->GetStageScore( GameRecorder::STATE_STAGE09 )
								+ GameRecorder::GetInstance()->GetStageScore( GameRecorder::STATE_STAGE10 );
			m_pNumCounter[static_cast<uint32_t>(DISP_SCENE03)]->AddValue( sceneScore );
		}
		break;
	case DISP_SCENE03:
		m_dispState = DISP_TOTAL;
		m_pNumCounter[static_cast<uint32_t>(DISP_TOTAL)]->AddValue( GameRecorder::GetInstance()->GetTotalScore() );
		break;
	case DISP_TOTAL:
		// 表示完了
		m_dispState = DISP_MAX;
		break;
	case DISP_MAX:
		return true;
		break;
	}
	return false;
}

