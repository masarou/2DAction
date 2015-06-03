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
: FlowBase( fileName )
, m_pMenuWindow( NULL )
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
	// 次の遷移先を常に監視
	if( m_pMenuWindow && !m_pMenuWindow->GetNextFlowStr().empty() ){
		StartFade( m_pMenuWindow->GetNextFlowStr().c_str() );
	}
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
, m_selectNo( 0 )
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
	for( uint32_t i = 0; i < SELECT_MAX ; ++i ){
		std::string animStr = "";
		switch( i ){
		default:
		case SELECT_START:
			animStr = "start";
			break;
		case SELECT_SCORE:
			animStr = "score";
			break;
		case SELECT_EXIT:
			animStr = "exit";
			break;
		}

		std::string partStr = "choice";
		partStr += '0' + i;
		SetAnim( partStr, animStr );
	}
	return true;
}

void TitleMenu::Update()
{
	if( !m_nextFlow.empty() ){
		// 次の遷移先が決まったのでなにもしない
		return;
	}

	CallPadEvent();
	for( uint32_t i = 0; i < SELECT_MAX; ++i){
		// カーソルが当たっていたらアニメ変更
		std::string anim = "default";
		if( m_selectNo == i ){
			anim = "spot";
		}
		std::string partStr = "choiceBG";
		partStr += '0' + i;
		SetAnim( partStr, anim );
	}
}


void TitleMenu::PadEventDecide()
{
	// 決定SE鳴らす
	SoundManager::GetInstance()->PlaySE("Decide");

	switch( m_selectNo ){
	default:
		break;
	case SELECT_START:
	m_nextFlow = "startgame";
		break;
	case SELECT_SCORE:
	m_nextFlow = "score";
		break;
	case SELECT_EXIT:
	m_nextFlow = "exit";
		break;
	}
}

void TitleMenu::PadEventUp()
{
}

void TitleMenu::PadEventDown()
{
}

void TitleMenu::PadEventRight()
{
	// カーソルSE鳴らす
	SoundManager::GetInstance()->PlaySE("Cursor");
	m_selectNo = (m_selectNo+1) % SELECT_MAX;
}
void TitleMenu::PadEventLeft()
{
	// カーソルSE鳴らす
	SoundManager::GetInstance()->PlaySE("Cursor");
	m_selectNo = (m_selectNo+(SELECT_MAX - 1)) % SELECT_MAX;
}
