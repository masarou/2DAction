/* ====================================================================== */
/**
 * @brief  
 *		ゲーム履歴表示クラス
 * @note
 *		
 */
/* ====================================================================== */

#include "FlowViewScore.h"
#include "Common/Utility/CommonGameUtility.h"

FlowBase *FlowViewScore::Create( const std::string &fileName )
{
	return NEW FlowViewScore(fileName);
}

FlowViewScore::FlowViewScore( const std::string &fileName )
: FlowBase(fileName)
{
	DEBUG_PRINT("FlowViewScore生成！！\n");
}


FlowViewScore::~FlowViewScore(void)
{
	DEBUG_PRINT("FlowResult削除！！\n");
}

bool FlowViewScore::Init()
{
	// 背景一枚絵作成
	m_pViewScoreTex = ViewScore2D::CreateViewScore2D();
	return true;
}

/* ================================================ */
/**
 * @brief	パッド操作関数
 */
/* ================================================ */
void FlowViewScore::PadEventCancel()
{
	// キャンセルSE鳴らす
	SoundManager::GetInstance()->PlaySE("Cancel");
	StartFade("return");
}


/* ====================================================================== */
/**
 * @brief  
 *		表示する数字カウンタ
 * @note
 *		
 */
/* ====================================================================== */
ViewScore2D *ViewScore2D::CreateViewScore2D()
{
	return NEW ViewScore2D();
}

ViewScore2D::ViewScore2D()
: TaskUnit("ViewScore2D")
{
	// 数字表示用画像情報
	m_numberInfo.Init();
	m_numberInfo.m_posOrigin.x = WINDOW_WIDTH - 200.0f;
	m_numberInfo.m_posOrigin.y = 100.0f;
	m_numberInfo.m_usePlayerOffset = false;
}

ViewScore2D::~ViewScore2D(void)
{
	m_textureHeadline.DeleteAndInit();
}

bool ViewScore2D::Init()
{
	// ヘッダー文字
	m_textureHeadline.Init();
	m_textureHeadline.m_pTex2D = NEW Game2DBase("scoreRanking.json");
	m_textureHeadline.m_texInfo.m_fileName = "scoreRanking.json";
	m_textureHeadline.m_texInfo.m_posOrigin.x = WINDOW_WIDTH / 2.0f;
	m_textureHeadline.m_texInfo.m_posOrigin.y = WINDOW_HEIGHT / 2.0f;
	m_textureHeadline.m_texInfo.m_usePlayerOffset = false;
	m_textureHeadline.m_pTex2D->SetDrawInfo(m_textureHeadline.m_texInfo);

	// ランキング取得
	Utility::GetSaveData( m_saveData );

	// ランキング描画
	for( uint32_t i = 0; i < Common::RANKING_RECORD_MAX; ++i ){
		m_pNumCounter[i] = NumberCounter::Create("numberLarge.json");
		m_numberInfo.m_posOrigin.y += 100.0f;
		m_pNumCounter[i]->SetDrawInfo( m_numberInfo );
		m_pNumCounter[i]->AddValue( m_saveData.m_scoreRanking[i] );
	}

	return true;
}

void ViewScore2D::Update()
{
	CallPadEvent();
}

void ViewScore2D::DrawUpdate()
{
	// 背景描画
	if( m_textureHeadline.m_pTex2D ){
		m_textureHeadline.m_pTex2D->DrawUpdate2D();
	}
}