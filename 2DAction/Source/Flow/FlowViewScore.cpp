/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		ゲーム履歴表示クラス
 */
/* ====================================================================== */

#include "FlowViewScore.h"
#include "Common/Utility/CommonGameUtility.h"
#include "System/Sound/SystemSoundManager.h"

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
 *
 * @note
 *		タイトル一枚絵クラス
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
	m_numberInfo.m_pos.x = WINDOW_WIDTH - 300.0f;
	m_numberInfo.m_pos.y = 100.0f;
	m_numberInfo.m_usePlayerOffset = false;
}

ViewScore2D::~ViewScore2D(void)
{
	m_textureResult.DeleteAndInit();
}

bool ViewScore2D::Init()
{
	// 背景作成
	m_textureResult.Init();
	m_textureResult.m_pTex2D = NEW Game2DBase("title.json");
	m_textureResult.m_texInfo.Init();
	m_textureResult.m_texInfo.m_pos.x = WINDOW_WIDTH / 2.0f;
	m_textureResult.m_texInfo.m_pos.y = WINDOW_HEIGHT / 2.0f;
	m_textureResult.m_texInfo.m_usePlayerOffset = false;
	m_textureResult.m_pTex2D->SetDrawInfo(m_textureResult.m_texInfo);

	// ランキング取得
	GetSaveRanking( m_scoreData );

	// ランキング描画
	for( uint32_t i = 0; i < Common::RANKING_RECORD_MAX; ++i ){
		m_pNumCounter[i] = NumberCounter::Create("number.json");
		m_numberInfo.m_pos.y += 100.0f;
		m_numberInfo.m_scale = math::Vector2(2.0f,2.0f);
		m_pNumCounter[i]->SetDrawInfo( m_numberInfo );
		m_pNumCounter[i]->AddValue( m_scoreData.m_scoreTimeAttack[i] );
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
	if( m_textureResult.m_pTex2D ){
		m_textureResult.m_pTex2D->DrawUpdate2D();
	}
}