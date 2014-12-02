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
#include "FlowResult.h"
#include "Game/GameScoreRecorder.h"

FlowBase *FlowResult::Create( const std::string &fileName )
{
	return NEW FlowResult(fileName);
}

FlowResult::FlowResult( const std::string &fileName )
: FlowBase(fileName)
, m_pResultTex(NULL)
{
	DEBUG_PRINT("FlowResult生成！！\n");
}


FlowResult::~FlowResult(void)
{
	DEBUG_PRINT("FlowResult削除！！\n");
}

bool FlowResult::Init()
{
	// 背景一枚絵作成
	m_pResultTex = Result2D::CreateResult2D();

	// 今回のプレイを反映させたランキング作成
	Common::SAVE_SCORE scoreLog;
	bool retVal = CheckSaveData( scoreLog );
	if( retVal ){
		UpdateSortRanking( scoreLog );
	}

	// プレイログ書き出し
	FILE *fpWrite = fopen( "playLog.dat", "wb" );
	if( fpWrite == NULL ){
		return 0;
	}
	fwrite( &scoreLog, sizeof(scoreLog), 1, fpWrite );
	fclose( fpWrite );

	return true;
}

/* ================================================ */
/**
 * @brief	パッド操作関数
 */
/* ================================================ */
void FlowResult::PadEventDecide()
{
	StartFade("proceed");
}

bool sortScore(const uint32_t &left, const uint32_t &rRight)
{
	return left > rRight;
}

void FlowResult::UpdateSortRanking( Common::SAVE_SCORE &scoreData )
{
	// 今までのスコアをpush
	std::vector<uint32_t> ranking;
	for( uint32_t i = 0; i < NUMBEROF(scoreData.m_scoreTimeAttack) ; ++i ){
		ranking.push_back(scoreData.m_scoreTimeAttack[i]);
	}

	// 今回のスコアをpush
	uint32_t scoreCurr = ScoreRecorder::GetInstance()->GetScore();
	ranking.push_back(scoreCurr);

	// 並び替え
	std::sort( ranking.begin(), ranking.end(), sortScore);
	
	for( uint32_t i = 0; i < NUMBEROF(scoreData.m_scoreTimeAttack) ; ++i ){
		scoreData.m_scoreTimeAttack[i] = ranking.at(i);
	}
}

bool FlowResult::CheckSaveData( Common::SAVE_SCORE &saveData )
{
	// ファイルがない場合もあるので一度開いて作成しておく
	FILE *fpCheck = fopen( "playLog.dat", "r" );
	if( !fpCheck ){
		// ファイルがない場合もあるので一度開いて作成しておく
		FILE *fpCreate = fopen( "playLog.dat", "a" );
		fclose( fpCreate );

		// デフォルトの値を詰めておく
		Common::SAVE_SCORE scoreLog = {
			{ 1000, 500, 300, 100, 0},
			{ 1000, 500, 300, 100, 0},
		};
		FILE *fpWriteDef = fopen( "playLog.dat", "wb" );
		if( fpWriteDef == NULL ){
			return false;
		}
		fwrite( &scoreLog, sizeof(scoreLog), 1, fpWriteDef );
		fclose( fpWriteDef );
	}
	else{
		// ファイルは存在している
		fclose( fpCheck );
	}

	// プレイログ読み込み
	FILE *fpRead = fopen( "playLog.dat", "rb" );
	if( fpRead == NULL ){
		return 0;
	}
	fread( &saveData, sizeof(saveData), 1, fpRead );
	fclose( fpRead );

	return true;
}


/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		タイトル一枚絵クラス
 */
/* ====================================================================== */
Result2D *Result2D::CreateResult2D()
{
	return NEW Result2D();
}

Result2D::Result2D()
: TaskUnit("Result2D")
, m_pNumCounter( NULL )
{
	m_textureResult.Init();
	m_textureResult.m_pTex2D = NEW Game2DBase("title.json");
	m_textureResult.m_texInfo.Init();
	m_textureResult.m_texInfo.m_pos.x = WINDOW_WIDTH / 2.0f;
	m_textureResult.m_texInfo.m_pos.y = WINDOW_HEIGHT / 2.0f;
	m_textureResult.m_texInfo.m_usePlayerOffset = false;
	m_textureResult.m_pTex2D->SetDrawInfo(m_textureResult.m_texInfo);

	// 数字表示用画像情報
	m_numberInfo.Init();
	m_numberInfo.m_pos.x = WINDOW_WIDTH / 2.0f;
	m_numberInfo.m_pos.y = WINDOW_HEIGHT / 2.0f;
	m_numberInfo.m_usePlayerOffset = false;
}

Result2D::~Result2D(void)
{
	m_textureResult.DeleteAndInit();
}

bool Result2D::Init()
{
	m_pNumCounter = NumberCounter::Create("number.json");

	//
	m_pNumCounter->SetDrawInfo( m_numberInfo );
	m_pNumCounter->AddValue( ScoreRecorder::GetInstance()->GetScore() );

	return true;
}

void Result2D::Update()
{
	CallPadEvent();
}

void Result2D::DrawUpdate()
{
	// 背景描画
	if( m_textureResult.m_pTex2D ){
		m_textureResult.m_pTex2D->DrawUpdate2D();
	}
}