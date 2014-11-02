
/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		タイトル画面管理クラス
 */
/* ====================================================================== */

#include "FlowTitle.h"

FlowBase *FlowTitle::Create(std::string fileName)
{
	return NEW FlowTitle(fileName);
}

FlowTitle::FlowTitle(std::string fileName)
	: FlowBase( fileName )
	, m_title2D( NULL )
{
	// 描画クラスセットアップ
	m_title2D = NEW Game2DBase("title.json");
	m_titleInfo.Init();
	m_titleInfo.m_pos.x = WINDOW_WIDTH / 2.0f;
	m_titleInfo.m_pos.y = WINDOW_HEIGHT / 2.0f;
	m_titleInfo.m_usePlayerOffset = false;
	m_title2D->SetDrawInfo(m_titleInfo);

	DEBUG_PRINT("FlowTitle生成！！\n");
}


FlowTitle::~FlowTitle(void)
{
	SAFE_DELETE( m_title2D );
	DEBUG_PRINT("FlowTitle削除！！\n");
}

//! 初期化処理記入
bool FlowTitle::Finish()
{
	return true;
}

bool FlowTitle::Init()
{
	return true;
}

void FlowTitle::FlowUpdate()
{
	CallPadEvent();

	// タイトル描画
	m_title2D->DrawUpdate2D();

	ChildUpdate();
}

void FlowTitle::PadEventDecide()
{
	StartFade("proceed");
}