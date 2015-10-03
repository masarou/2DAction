/* ====================================================================== */
/**
 * @brief  
 *		ゲームクリアー後リンク表示画面
 * @note
 *		
 */
/* ====================================================================== */
#include "FlowGameClearShowAddress.h"


FlowBase *FlowGameClear::Create( const std::string &fileName )
{
	return NEW FlowGameClear(fileName);
}

FlowGameClear::FlowGameClear( const std::string &fileName )
: FlowMenuBase(fileName)
{
	DEBUG_PRINT("FlowGameClear生成！！\n");
}


FlowGameClear::~FlowGameClear(void)
{
	DEBUG_PRINT("FlowResult削除！！\n");
}

bool FlowGameClear::Init()
{
	// 背景一枚絵作成
	m_pMenuWindow = ShowAddressMenu::CreateViewScore2D( "GameClearShowAddress.json" );
	return true;
}


ShowAddressMenu *ShowAddressMenu::CreateViewScore2D( const std::string &fileName )
{
	return NEW ShowAddressMenu(fileName);
}

ShowAddressMenu::ShowAddressMenu( const std::string &fileName )
	: MenuWindow(fileName, Common::PRIORITY_NORMAL, WINDOW_CENTER )
{
	DEBUG_PRINT("ShowAddressMenu生成！！\n");
}


ShowAddressMenu::~ShowAddressMenu(void)
{
	DEBUG_PRINT("ShowAddressMenu削除！！\n");
}

void ShowAddressMenu::UpdateMenu()
{
	if( !GetNextFlowStr().empty() ){
		// 次の遷移先が決まったのでなにもしない
		return;
	}

	CallPadEvent();
}

/* ================================================ */
/**
 * @brief	パッド操作関数
 */
/* ================================================ */
void ShowAddressMenu::PadEventDecide()
{
	// キャンセルSE鳴らす
	SoundManager::GetInstance()->PlaySE("Cancel");
	SetNextFlowStr( "title" );
}


