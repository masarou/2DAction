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
{
	m_textureTitle.Init();

	// 描画クラスセットアップ
	m_textureTitle.m_pTex2D = NEW Game2DBase("title.json");
	m_textureTitle.m_texInfo.Init();
	m_textureTitle.m_texInfo.m_pos.x = WINDOW_WIDTH / 2.0f;
	m_textureTitle.m_texInfo.m_pos.y = WINDOW_HEIGHT / 2.0f;
	m_textureTitle.m_texInfo.m_usePlayerOffset = false;
	m_textureTitle.m_pTex2D->SetDrawInfo(m_textureTitle.m_texInfo);
}

Title2D::~Title2D(void)
{
	m_textureTitle.DeleteAndInit();
}

void Title2D::DrawUpdate()
{
	if( m_textureTitle.m_pTex2D ){
		m_textureTitle.m_pTex2D->DrawUpdate2D();
	}
}