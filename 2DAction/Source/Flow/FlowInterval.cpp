/* ====================================================================== */
/**
 * @brief  
 *		ゲーム終了後リトライ、次に進むか確認する画面
 * @note
 *		
 */
/* ====================================================================== */
#include "FlowInterval.h"
#include "Game/GameRecorder.h"

FlowBase *FlowInterval::Create( const std::string &fileName )
{
	return NEW FlowInterval(fileName);
}

FlowInterval::FlowInterval( const std::string &fileName )
: FlowMenuBase(fileName)
{
	DEBUG_PRINT("FlowInterval生成！！\n");
}


FlowInterval::~FlowInterval(void)
{
	DEBUG_PRINT("FlowInterval削除！！\n");
}

bool FlowInterval::Init()
{
	// 一枚絵作成
	m_pMenuWindow = IntervalMenu::CreateIntervalMenu( "MenuInterval.json" );
	return true;
}

/* ================================================ */
/**
 * @brief	次に進むべきラベル取得関数
 */
/* ================================================ */
void FlowInterval::UpdateFlowAfterChildTask()
{
}


/* ====================================================================== */
/**
 * @brief  
 *		FlowIntervalの表示物中身
 * @note
 *		
 */
/* ====================================================================== */
IntervalMenu *IntervalMenu::CreateIntervalMenu( const std::string &readMenuJson )
{
	return NEW IntervalMenu( readMenuJson );
}

IntervalMenu::IntervalMenu( const std::string &readMenuJson )
: MenuWindow( readMenuJson )
{
}

IntervalMenu::~IntervalMenu(void)
{
}


bool IntervalMenu::InitMenu()
{
	// 方向キーのパッドイベントはPUSHで呼ぶように設定
	SetPadButtonState( InputWatcher::BUTTON_UP,		InputWatcher::EVENT_PUSH );
	SetPadButtonState( InputWatcher::BUTTON_DOWN,	InputWatcher::EVENT_PUSH );
	SetPadButtonState( InputWatcher::BUTTON_RIGHT,	InputWatcher::EVENT_PUSH );
	SetPadButtonState( InputWatcher::BUTTON_LEFT,	InputWatcher::EVENT_PUSH );

	//
	SetChoiceSelect( SELECT_NEXT_GAME );

	return true;
}

void IntervalMenu::PadEventDecide()
{
	// 決定SE鳴らす
	SoundManager::GetInstance()->PlaySE("Decide");

	if( GetSelectedNo() == IntervalMenu::SELECT_TITLE ){
		SetNextFlowStr( "title" );
	}
	else{
		switch( FlowManager::GetInstance()->GetCurrentFlowKind() ){
		default:
			DEBUG_ASSERT( 0, "想定外のフロー" );
			// とりあえずタイトルへ
			SetNextFlowStr( "title" );
			break;
		case Common::FLOW_STAGE01:
			SetNextFlowStr( "nextgame02" );
			break;
		case Common::FLOW_STAGE02:
			SetNextFlowStr( "nextgame03" );
			break;
		case Common::FLOW_STAGE03:
			SetNextFlowStr( "nextgame04" );
			break;
		case Common::FLOW_STAGE04:
			SetNextFlowStr( "nextgame05" );
			break;
		case Common::FLOW_STAGE05:
			SetNextFlowStr( "nextgame06" );
			break;
		case Common::FLOW_STAGE06:
			SetNextFlowStr( "nextgame07" );
			break;
		case Common::FLOW_STAGE07:
			SetNextFlowStr( "nextgame08" );
			break;
		case Common::FLOW_STAGE08:
			SetNextFlowStr( "nextgame09" );
			break;
		case Common::FLOW_STAGE09:
			SetNextFlowStr( "nextgame10" );
			break;
		}
	}
}

void IntervalMenu::PadEventRight()
{
	// カーソルSE鳴らす
	SoundManager::GetInstance()->PlaySE("Cursor");
	
	// 選択しているIndex更新
	uint32_t selectNo = ( GetSelectedNo() + 1 ) % SELECT_RETRY_MAX;
	SetSelectNum( selectNo );

	// 選択肢描画更新
	SetChoiceSelect( GetSelectedNo() );
}
void IntervalMenu::PadEventLeft()
{
	// カーソルSE鳴らす
	SoundManager::GetInstance()->PlaySE("Cursor");

	// 選択しているIndex更新
	uint32_t selectNo = ( GetSelectedNo() + (SELECT_RETRY_MAX - 1) ) % SELECT_MAX;
	SetSelectNum( selectNo );

	// 選択肢描画更新
	SetChoiceSelect( GetSelectedNo() );
}

void IntervalMenu::UpdateMenu()
{
	if( !GetNextFlowStr().empty() ){
		// 次の遷移先が決まったのでなにもしない
		return;
	}

	CallPadEvent();
}

void IntervalMenu::SetChoiceSelect( uint32_t choiceIndex )
{
	for( uint32_t i = 0; i < SELECT_MAX; ++i){
		// カーソルが当たっていたらアニメ変更
		std::string choiceStr = "choice";
		std::string choiceAnimStr = "";
		std::string bgStr = "choiceBG";
		std::string bgAnimStr = "default";
		switch( i ){
		default:
		case SELECT_NEXT_GAME:
			choiceAnimStr = "next";
			break;
		case SELECT_TO_TITLE:
			choiceAnimStr = "title";
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
