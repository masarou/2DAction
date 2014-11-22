/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		タイトル画面管理クラス
 */
/* ====================================================================== */

#include "FlowTitle.h"
#include "System/Sound/SystemSoundManager.h"

FlowBase *FlowTitle::Create( const std::string &fileName )
{
	return NEW FlowTitle(fileName);
}

FlowTitle::FlowTitle( const std::string &fileName )
: FlowBase( fileName )
, m_pTitleTex( NULL )
{
	DEBUG_PRINT("FlowTitle生成！！\n");
}


FlowTitle::~FlowTitle(void)
{
	DEBUG_PRINT("FlowTitle削除！！\n");
}

bool FlowTitle::Finish()
{
	return true;
}

bool FlowTitle::Init()
{
	// 一枚絵作成
	m_pTitleTex = Title2D::CreateTitle2D();
	return true;
}

void FlowTitle::PadEventDecide()
{
	// 決定SE鳴らす
	SoundManager::GetInstance()->PlaySE("Decide");
	StartFade("proceed");
}


/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		タイトル一枚絵クラス
 */
/* ====================================================================== */
Title2D *Title2D::CreateTitle2D()
{
	return NEW Title2D();
}

Title2D::Title2D()
: TaskUnit("Title2D")
, m_title2D( NULL )
{
	// 描画クラスセットアップ
	m_title2D = NEW Game2DBase("title.json");
	m_titleInfo.Init();
	m_titleInfo.m_pos.x = WINDOW_WIDTH / 2.0f;
	m_titleInfo.m_pos.y = WINDOW_HEIGHT / 2.0f;
	m_titleInfo.m_usePlayerOffset = false;
	m_title2D->SetDrawInfo(m_titleInfo);
}

Title2D::~Title2D(void)
{
	SAFE_DELETE( m_title2D );
}

void Title2D::DrawUpdate()
{
	if( m_title2D ){
		m_title2D->DrawUpdate2D();
	}
}