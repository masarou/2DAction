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
 *			ステージ結果の中身カウンタクラス
 * @note
 *		
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
	m_textureBG.DeleteAndInit();
}

bool Result2D::Init()
{
	// 背景セット
	m_textureBG.Init();
	m_textureBG.m_pTex2D = NEW Game2DBase("title.json");
	m_textureBG.m_texInfo.Init();
	m_textureBG.m_texInfo.m_fileName = "title.json";
	m_textureBG.m_texInfo.m_posOrigin.x = WINDOW_WIDTH / 2.0f;
	m_textureBG.m_texInfo.m_posOrigin.y = WINDOW_HEIGHT / 2.0f;
	m_textureBG.m_texInfo.m_usePlayerOffset = false;
	m_textureBG.m_texInfo.m_prioity = PRIORITY_LOWEST;
	m_textureBG.m_pTex2D->SetDrawInfo(m_textureBG.m_texInfo);

	// ステータスメニューのパーツ情報取得
	Utility::GetPartsInfoFromJson( "stageResult.json", m_partsMap );

	// 画面フレームセット
	m_textureResult.Init();
	m_textureResult.m_pTex2D = NEW Game2DBase("stageResult.json");
	m_textureResult.m_texInfo.Init();
	m_textureResult.m_texInfo.m_fileName = "stageResult.json";
	m_textureResult.m_texInfo.m_posOrigin.x = WINDOW_WIDTH / 2.0f;
	m_textureResult.m_texInfo.m_posOrigin.y = WINDOW_HEIGHT / 2.0f;
	m_textureResult.m_texInfo.m_usePlayerOffset = false;
	m_textureBG.m_texInfo.m_prioity = PRIORITY_LOW;
	m_textureResult.m_pTex2D->SetDrawInfo(m_textureResult.m_texInfo);

	// 数字カウンタの初期化
	m_pNumCounterResult = NumberCounter::Create("numberLarge.json");
	m_pNumCounterBonus = NumberCounter::Create("numberLarge.json");
	m_pNumCounterTotal = NumberCounter::Create("numberLarge.json");

	// 数字表示用画像情報
	m_numberInfo.Init();
	m_numberInfo.m_usePlayerOffset = false;

	// 数字表示用画像情報セット
	m_numberInfo.m_posOrigin = GetPartsPos( "strNumber01" );
	m_pNumCounterResult->SetDrawInfo( m_numberInfo );
	m_numberInfo.m_posOrigin = GetPartsPos( "strNumber02" );
	m_pNumCounterBonus->SetDrawInfo( m_numberInfo );
	m_numberInfo.m_posOrigin = GetPartsPos( "strNumber04" );
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
			m_pNumCounterBonus->AddValue( GetStageClearBonus() );		
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
	if( m_textureBG.m_pTex2D ){
		m_textureBG.m_pTex2D->DrawUpdate2D();
	}
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

		m_pNumCounterBonus->AddValue( GetStageClearBonus() );
		break;
	case DISP_BONUS:
		// カウントアニメ終了
		m_pNumCounterBonus->CountAnimEnd();
		m_dispState = DISP_TOTAL;

		m_pNumCounterTotal->AddValue( m_pNumCounterResult->GetValue() + m_pNumCounterBonus->GetValue() );
		break;
	case DISP_TOTAL:
		// カウントアニメ終了
		m_pNumCounterTotal->CountAnimEnd();
		m_dispState = DISP_ALL;
		
		// ここで求めたボーナスをステージスコアに加算
		GameRecorder::GetInstance()->AddScoreBonus( m_pNumCounterBonus->GetValue() );
		break;
	case DISP_ALL:

		break;
	}
}

uint32_t Result2D::GetStageClearBonus() const
{
	// ボーナス算出方法
	// コンボ数×ユーザーの残りライフ = Bonus

	uint32_t retVal = 0;
	uint32_t stageComboMax = GameRecorder::GetInstance()->GetMaxComboNumOfStage();

	enum SCORE_STAGE {
		SCORE_01 = 100,
		SCORE_02 = 200,
		SCORE_03 = 300,
		SCORE_04 = 400,
		SCORE_05 = 500,
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

const math::Vector2 Result2D::GetPartsPos( const std::string name ) const
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

const Common::PARTS_INFO &Result2D::GetPartsInfo( const std::string name ) const
{
	auto it = m_partsMap.find( name.c_str() );
	if( it != m_partsMap.end() ){
		return (*it).second;
	}
	DEBUG_ASSERT( 0, "パーツが見つかりません\n" );
	return (*m_partsMap.begin()).second;
}