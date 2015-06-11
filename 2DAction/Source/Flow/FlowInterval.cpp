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
: FlowBase(fileName)
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
	// 次の遷移先を常に監視
	if( m_pMenuWindow && !m_pMenuWindow->GetNextFlowStr().empty() ){
		StartFade( m_pMenuWindow->GetNextFlowStr().c_str() );
	}
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
, m_selectNo( 0 )
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

	// 選択肢項目のセットアップ
	for( uint32_t i = 0; i < SELECT_MAX ; ++i ){
		std::string animStr = "";
		switch( i ){
		case SELECT_NEXT_GAME:
			animStr = "next";
			break;
		case SELECT_TO_TITLE:
			animStr = "title";
			break;
		default:
			DEBUG_ASSERT( 0, "選択肢番号が想定外");
			break;
		}

		std::string partStr = "choice";
		partStr += '0' + i;
		SetAnim( partStr, animStr );
	}
	return true;
}

void IntervalMenu::PadEventDecide()
{
	// 決定SE鳴らす
	SoundManager::GetInstance()->PlaySE("Decide");

	if( m_selectNo == IntervalMenu::SELECT_TITLE ){
		m_nextFlow = "title";
	}
	else{
		GameRecorder *pRecorder = GameRecorder::GetInstance();
		if( pRecorder ){
			switch( pRecorder->GetGameStateOfProgress() ){
			case GameRecorder::STATE_TITLE:
			case GameRecorder::STATE_POWER_UP:
			case GameRecorder::STATE_STAGE03:
			default:
				DEBUG_ASSERT( 0, "想定外のフロー" );
				// とりあえずタイトルへ
				m_nextFlow = "title";
				break;
			case GameRecorder::STATE_STAGE01:
				m_nextFlow = "nextgame02";
				break;
			case GameRecorder::STATE_STAGE02:
				m_nextFlow = "nextgame03";
				break;
			}
		}
	}
}

void IntervalMenu::PadEventRight()
{
	// カーソルSE鳴らす
	SoundManager::GetInstance()->PlaySE("Cursor");
	m_selectNo = (m_selectNo+1) % SELECT_RETRY_MAX;
}
void IntervalMenu::PadEventLeft()
{
	// カーソルSE鳴らす
	SoundManager::GetInstance()->PlaySE("Cursor");
	m_selectNo = (m_selectNo+(SELECT_RETRY_MAX - 1)) % SELECT_RETRY_MAX;
}

void IntervalMenu::UpdateMenu()
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
