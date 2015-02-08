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
#include "FlowStageResult.h"
#include "FlowManager.h"
#include "Game/GameRecorder.h"
#include "Common/Utility/CommonGameUtility.h"

FlowBase *FlowStageResult::Create( const std::string &fileName )
{
	return NEW FlowStageResult(fileName);
}

FlowStageResult::FlowStageResult( const std::string &fileName )
: FlowBase(fileName)
, m_pResultTex(NULL)
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
	m_pResultTex = Result2D::CreateResult2D();
	return true;
}

/* ================================================ */
/**
 * @brief	パッド操作関数
 */
/* ================================================ */
void FlowStageResult::PadEventDecide()
{
	if( m_pResultTex && m_pResultTex->ProceedNext() ){
		// 次に進む
		GameRecorder *pRecorder = GameRecorder::GetInstance();
		if( !pRecorder ){
			DEBUG_ASSERT( 0, "GameRecorder is NULL");
			return;
		}
		switch( pRecorder->GetGameStateOfProgress() ){
		case GameRecorder::STATE_TITLE:
		default:
			DEBUG_ASSERT( 0, "想定外のフロー" );
			// とりあえずタイトルへ
			StartFade( "title" );
			break;
		case GameRecorder::STATE_STAGE01:
		case GameRecorder::STATE_STAGE02:
			StartFade( "interval" );
			break;
		case GameRecorder::STATE_STAGE03:
			StartFade( "totalresult" );	// すべてのステージ終了
			break;
		}	
		// 決定SE鳴らす
		SoundManager::GetInstance()->PlaySE("Decide");
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
Result2D *Result2D::CreateResult2D()
{
	return NEW Result2D();
}

Result2D::Result2D()
: TaskUnit("Result2D")
, m_dispState(DISP_RESULT)
, m_pNumCounterResult( NULL )
, m_pNumCounterBonus( NULL )
, m_pNumCounterTotal( NULL )
{
}

Result2D::~Result2D(void)
{
	m_textureResult.DeleteAndInit();
}

bool Result2D::Init()
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

void Result2D::Update()
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

void Result2D::DrawUpdate()
{
	// 背景描画
	if( m_textureResult.m_pTex2D ){
		m_textureResult.m_pTex2D->DrawUpdate2D();
	}
}

void Result2D::PadEventDecide()
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
