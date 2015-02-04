/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		ゲーム結果画面クラス
 */
/* ====================================================================== */

#include <fstream>
#include <algorithm>
#include "FlowTotalResult.h"
#include "Game/GameRecorder.h"
#include "Common/Utility/CommonGameUtility.h"

FlowBase *FlowTotalResult::Create( const std::string &fileName )
{
	return NEW FlowTotalResult(fileName);
}

FlowTotalResult::FlowTotalResult( const std::string &fileName )
: FlowBase(fileName)
, m_pResultTex(NULL)
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
	m_pResultTex = TotalResult2D::CreateTotalResult2D();
	return true;
}

bool FlowTotalResult::FinishFlow()
{
	// 常駐物で初期化が必要なものリセット
	Utility::GameInitALL();

	return true;
}

/* ================================================ */
/**
 * @brief	パッド操作関数
 */
/* ================================================ */
void FlowTotalResult::PadEventDecide()
{
	if( m_pResultTex && m_pResultTex->ProceedNext() ){
		StartFade("title");

		// 今回のプレイを反映させたランキング作成
		Common::SAVE_SCORE scoreLog;
		bool retVal = Utility::GetSaveRanking( scoreLog );
		if( retVal ){
			UpdateSortRanking( scoreLog );
		}
	
		// プレイログ書き出し
		FILE *fpWrite = fopen( "playLog.dat", "wb" );
		if( fpWrite == NULL ){
			return;
		}
		fwrite( &scoreLog, sizeof(scoreLog), 1, fpWrite );
		fclose( fpWrite );
	}
}

bool sortScore(const uint32_t &left, const uint32_t &rRight)
{
	return left > rRight;
}

void FlowTotalResult::UpdateSortRanking( Common::SAVE_SCORE &scoreData )
{
	// 今までのスコアをpush
	std::vector<uint32_t> ranking;
	for( uint32_t i = 0; i < NUMBEROF(scoreData.m_scoreTimeAttack) ; ++i ){
		ranking.push_back(scoreData.m_scoreTimeAttack[i]);
	}

	// 今回のスコアをpush
	uint32_t scoreCurr = GameRecorder::GetInstance()->GetScore();
	ranking.push_back(scoreCurr);

	// 並び替え
	std::sort( ranking.begin(), ranking.end(), sortScore);
	
	for( uint32_t i = 0; i < NUMBEROF(scoreData.m_scoreTimeAttack) ; ++i ){
		scoreData.m_scoreTimeAttack[i] = ranking.at(i);
	}
}

/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		タイトル一枚絵クラス
 */
/* ====================================================================== */
TotalResult2D *TotalResult2D::CreateTotalResult2D()
{
	return NEW TotalResult2D();
}

TotalResult2D::TotalResult2D()
: TaskUnit("TotalResult2D")
, m_dispState(DISP_RESULT)
, m_pNumCounterResult( NULL )
, m_pNumCounterBonus( NULL )
, m_pNumCounterTotal( NULL )
{
}

TotalResult2D::~TotalResult2D(void)
{
	m_textureResult.DeleteAndInit();
}

bool TotalResult2D::Init()
{
	// 背景セット
	m_textureResult.Init();
	m_textureResult.m_pTex2D = NEW Game2DBase("title.json");
	m_textureResult.m_texInfo.Init();
	m_textureResult.m_texInfo.m_posOrigin.x = WINDOW_WIDTH / 2.0f;
	m_textureResult.m_texInfo.m_posOrigin.y = WINDOW_HEIGHT / 2.0f;
	m_textureResult.m_texInfo.m_usePlayerOffset = false;
	m_textureResult.m_pTex2D->SetDrawInfo(m_textureResult.m_texInfo);

	// 数字カウンタの初期化
	m_pNumCounterResult = NumberCounter::Create("number.json");
	m_pNumCounterBonus = NumberCounter::Create("number.json");
	m_pNumCounterTotal = NumberCounter::Create("number.json");

	// 数字表示用画像情報
	m_numberInfo.Init();
	m_numberInfo.m_posOrigin.x = WINDOW_WIDTH / 2.0f + 500.0f;
	m_numberInfo.m_posOrigin.y = 150.0f;
	m_numberInfo.m_scale = math::Vector2(2.0f,2.0f);
	m_numberInfo.m_usePlayerOffset = false;

	// 数字表示用画像情報セット
	m_pNumCounterResult->SetDrawInfo( m_numberInfo );
	m_numberInfo.m_posOrigin.y += 120.0f;
	m_pNumCounterBonus->SetDrawInfo( m_numberInfo );
	m_numberInfo.m_posOrigin.y += 200.0f;
	m_pNumCounterTotal->SetDrawInfo( m_numberInfo );

	// 敵を倒して得た得点をセット
	m_pNumCounterResult->AddValue( GameRecorder::GetInstance()->GetScore() );

	return true;
}

void TotalResult2D::Update()
{
	CallPadEvent();

	// アニメカウントが終わっているなら次のステップに進む
	switch(m_dispState){
	case DISP_RESULT:
		if( !m_pNumCounterResult->IsPlayCountAnim() ){
			m_dispState = DISP_BONUS;
			m_pNumCounterBonus->AddValue( GameRecorder::GetInstance()->GetScore() );
		}
		break;
	case DISP_BONUS:
		if( !m_pNumCounterBonus->IsPlayCountAnim() ){
			m_dispState = DISP_TOTAL;
			m_pNumCounterTotal->AddValue( m_pNumCounterResult->GetValue() + m_pNumCounterBonus->GetValue() );
		}
		break;
	case DISP_TOTAL:
		if( !m_pNumCounterTotal->IsPlayCountAnim() ){
			m_dispState = DISP_ALL;
		}
		break;
	case DISP_ALL:

		break;
	}

}

void TotalResult2D::DrawUpdate()
{
	// 背景描画
	if( m_textureResult.m_pTex2D ){
		m_textureResult.m_pTex2D->DrawUpdate2D();
	}
}

void TotalResult2D::PadEventDecide()
{

	switch(m_dispState){
	case DISP_RESULT:
		// カウントアニメ終了
		m_pNumCounterResult->CountAnimEnd();
		m_dispState = DISP_BONUS;

		m_pNumCounterBonus->AddValue( GameRecorder::GetInstance()->GetScore() );
		break;
	case DISP_BONUS:
		// カウントアニメ終了
		m_pNumCounterBonus->CountAnimEnd();
		m_dispState = DISP_TOTAL;

		m_pNumCounterTotal->AddValue( GameRecorder::GetInstance()->GetScore() );
		break;
	case DISP_TOTAL:
		// カウントアニメ終了
		m_pNumCounterTotal->CountAnimEnd();
		m_dispState = DISP_ALL;
		break;
	case DISP_ALL:

		break;
	}
}
