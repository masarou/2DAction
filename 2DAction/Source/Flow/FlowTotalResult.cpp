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
	uint32_t scoreCurr = GameRecorder::GetInstance()->GetTotalScore();
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
 *		表示するスコアそのもの
 */
/* ====================================================================== */
TotalResult2D *TotalResult2D::CreateTotalResult2D()
{
	return NEW TotalResult2D();
}

TotalResult2D::TotalResult2D()
: TaskUnit("TotalResult2D")
, m_dispState(DISP_STAGE01)
{
	for( uint32_t i = 0; i < DISP_MAX ; ++i ){
		m_pNumCounter[i] = NULL;
	}
}

TotalResult2D::~TotalResult2D(void)
{
	m_textureBG.DeleteAndInit();
	m_textureResult.DeleteAndInit();
}

bool TotalResult2D::Init()
{
	// 背景セット
	m_textureBG.Init();
	m_textureBG.m_pTex2D = NEW Game2DBase("titleBg.json");
	m_textureBG.m_texInfo.m_fileName = "titleBg.json";
	m_textureBG.m_texInfo.m_posOrigin.x = WINDOW_WIDTH / 2.0f;
	m_textureBG.m_texInfo.m_posOrigin.y = WINDOW_HEIGHT / 2.0f;
	m_textureBG.m_texInfo.m_usePlayerOffset = false;
	m_textureBG.m_texInfo.m_prioity = PRIORITY_LOWEST;
	m_textureBG.m_pTex2D->SetDrawInfo(m_textureBG.m_texInfo);

	// ステータスメニューのパーツ情報取得
	Utility::GetPartsInfoFromJson( "gameResult.json", m_partsMap );

	// 画面フレームセット
	m_textureResult.Init();
	m_textureResult.m_pTex2D = NEW Game2DBase("gameResult.json");
	m_textureResult.m_texInfo.m_fileName = "gameResult.json";
	m_textureResult.m_texInfo.m_posOrigin.x = WINDOW_WIDTH / 2.0f;
	m_textureResult.m_texInfo.m_posOrigin.y = WINDOW_HEIGHT / 2.0f;
	m_textureResult.m_texInfo.m_usePlayerOffset = false;
	m_textureBG.m_texInfo.m_prioity = PRIORITY_LOW;
	m_textureResult.m_pTex2D->SetDrawInfo(m_textureResult.m_texInfo);

	// 数字表示用画像情報
	m_numberInfo.Init();
	m_numberInfo.m_usePlayerOffset = false;

	// 数字カウンタの初期化
	// 数字表示用画像情報セット
	static const std::string s_partsStr[DISP_MAX] = {
		"strNumber01",
		"strNumber02",
		"strNumber03",
		"strNumber04",
	};
	for( uint32_t i = 0; i < DISP_MAX ; ++i ){
		m_pNumCounter[i] = NumberCounter::Create("numberLarge.json");
		m_numberInfo.m_posOrigin = GetPartsPos( s_partsStr[i] );
		m_pNumCounter[i]->SetDrawInfo( m_numberInfo );
	}

	// 敵を倒して得た得点をセット
	m_pNumCounter[static_cast<uint32_t>(DISP_STAGE01)]->AddValue( GameRecorder::GetInstance()->GetStageScore( GameRecorder::STATE_STAGE01 ) );

	return true;
}

void TotalResult2D::Update()
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

void TotalResult2D::DrawUpdate()
{
	// 背景描画
	if( m_textureBG.m_pTex2D ){
		m_textureBG.m_pTex2D->DrawUpdate2D();
	}
	if( m_textureResult.m_pTex2D ){
		m_textureResult.m_pTex2D->DrawUpdate2D();
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

		break;
	}
}

const math::Vector2 TotalResult2D::GetPartsPos( const std::string name ) const
{
	// ステータスメニューの左上座標取得
	const TEX_INIT_INFO &resultMenuInfo = TextureResourceManager::GetInstance()->GetLoadTextureInfo( m_textureResult.m_texInfo.m_fileName.c_str() );
	math::Vector2 retPos = m_textureResult.m_texInfo.m_posOrigin;
	retPos -= math::Vector2( resultMenuInfo.m_sizeWidth / 2.0f, resultMenuInfo.m_sizeHeight / 2.0f );

	// そこからパーツの位置を足し算
	Common::PARTS_INFO info = GetPartsInfo(name);
	retPos += info.m_pos;
	return retPos;
}

const Common::PARTS_INFO &TotalResult2D::GetPartsInfo( const std::string name ) const
{
	auto it = m_partsMap.find( name.c_str() );
	if( it != m_partsMap.end() ){
		return (*it).second;
	}
	DEBUG_ASSERT( 0, "パーツが見つかりません\n" );
	return (*m_partsMap.begin()).second;
}