/* ====================================================================== */
/**
 * @brief  
 *		タイトル画面管理クラス
 * @note
 *		
 */
/* ====================================================================== */

#include "FlowTitle.h"
#include "Common/Utility/CommonGameUtility.h"

FlowBase *FlowTitle::Create( const std::string &fileName )
{
	return NEW FlowTitle(fileName);
}

FlowTitle::FlowTitle( const std::string &fileName )
: FlowMenuBase( fileName )
{
	DEBUG_PRINT("FlowTitle生成！！\n");
}


FlowTitle::~FlowTitle(void)
{
	DEBUG_PRINT("FlowTitle削除！！\n");
}

bool FlowTitle::Init()
{
	// ゲームスコア等初期化
	Utility::GameInitALL();

	// 選択肢管理クラス
	m_pMenuWindow = TitleMenu::Create( "MenuTitle.json" );

	// BGM再生開始
	SoundManager::GetInstance()->PlayBGM( "title" );

	return true;
}

void FlowTitle::UpdateFlowAfterChildTask()
{

}

/* ====================================================================== */
/**
 * @brief  
 *		タイトル選択肢クラス
 * @note
 *		
 */
/* ====================================================================== */
TitleMenu *TitleMenu::Create( const std::string &readMenuJson )
{
	return NEW TitleMenu( readMenuJson );
}

TitleMenu::TitleMenu( const std::string &readMenuJson )
: MenuWindow( readMenuJson )
{
}

TitleMenu::~TitleMenu(void)
{
}

bool TitleMenu::InitMenu()
{
	// 方向キーのパッドイベントはPUSHで呼ぶように設定
	SetPadButtonState( InputWatcher::BUTTON_UP,		InputWatcher::EVENT_PUSH );
	SetPadButtonState( InputWatcher::BUTTON_DOWN,	InputWatcher::EVENT_PUSH );
	SetPadButtonState( InputWatcher::BUTTON_RIGHT,	InputWatcher::EVENT_PUSH );
	SetPadButtonState( InputWatcher::BUTTON_LEFT,	InputWatcher::EVENT_PUSH );

	// 選択肢項目のセットアップ
	SetChoiceSelect( SELECT_START );
	return true;
}

void TitleMenu::UpdateMenu()
{
	if( !GetNextFlowStr().empty() ){
		// 次の遷移先が決まったのでなにもしない
		return;
	}

	CallPadEvent();
}


void TitleMenu::SetChoiceSelect( uint32_t choiceIndex )
{
	for( uint32_t i = 0; i < SELECT_MAX; ++i){
		// カーソルが当たっていたらアニメ変更
		std::string choiceStr = "choice";
		std::string choiceAnimStr = "";
		std::string bgStr = "choiceBG";
		std::string bgAnimStr = "default";
		switch( i ){
		default:
		case SELECT_START:
			choiceAnimStr = "start";
			break;
		case SELECT_SCORE:
			choiceAnimStr = "score";
			break;
		case SELECT_EXIT:
			choiceAnimStr = "exit";
			break;
		}
		if( choiceIndex == i ){
			choiceAnimStr += "Select";
			bgAnimStr = "spot";
		}
		choiceStr += '0' + i;
		bgStr += '0' + i;
		SetAnim( choiceStr, choiceAnimStr );
		SetAnim( bgStr, bgAnimStr );
	}
}

void TitleMenu::PadEventDecide()
{
	// 決定SE鳴らす
	SoundManager::GetInstance()->PlaySE("Decide");

	switch( GetSelectedNo() ){
	default:
		break;
	case SELECT_START:
	SetNextFlowStr( "startgame" );
		break;
	case SELECT_SCORE:
	SetNextFlowStr( "score" );
		break;
	case SELECT_EXIT:
	SetNextFlowStr( "exit" );
		break;
	}
}

void TitleMenu::PadEventUp()
{
	// カーソルSE鳴らす
	SoundManager::GetInstance()->PlaySE("Cursor");
	
	// 選択しているIndex更新
	uint32_t selectNo = ( GetSelectedNo() + (SELECT_MAX - 1) ) % SELECT_MAX;
	SetSelectNum( selectNo );
	
	// choice更新
	SetChoiceSelect( GetSelectedNo() );
}

void TitleMenu::PadEventDown()
{
	// カーソルSE鳴らす
	SoundManager::GetInstance()->PlaySE("Cursor");
	
	// 選択しているIndex更新
	uint32_t selectNo = ( GetSelectedNo() + 1 ) % SELECT_MAX;
	SetSelectNum( selectNo );
	
	// choice更新
	SetChoiceSelect( GetSelectedNo() );
}

void TitleMenu::PadEventRight()
{

}
void TitleMenu::PadEventLeft()
{
}
