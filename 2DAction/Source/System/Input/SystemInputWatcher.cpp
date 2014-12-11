
/* ====================================================================== */
/**
 * @brief  パッド入力監視クラス
 *
 * @note
 *
*/
/* ====================================================================== */

#include "System/Input/SystemInputWatcher.h"

//! スティックの無効範囲
#define STICK_INVALID_RANGE 250

InputWatcher::InputWatcher( const uint32_t &padIndex )
: m_buttonState(0)
, m_preButtonState(0)
, m_watchPadIndex(padIndex)
{
	for(uint32_t i = 0; i < STICK_MAX; ++i){
		STICK_INFO tmp;
		tmp.Init();
		m_stickInfo[i] = tmp;
	}

	for(uint32_t i = 0; i < BUTTON_MAX; ++i){
		m_buttonEventStatus[i] = EVENT_PUSH;
	}
	m_buttonEventStatus[UP] = EVENT_PRESS;
	m_buttonEventStatus[DOWN] = EVENT_PRESS;
	m_buttonEventStatus[RIGHT] = EVENT_PRESS;
	m_buttonEventStatus[LEFT] = EVENT_PRESS;
}


InputWatcher::~InputWatcher(void)
{
	std::vector<CommandTable*>::iterator it = m_vCommandTable.begin();
	while(m_vCommandTable.empty() == 0){
		CommandTable *task = *it;
		it = m_vCommandTable.erase(it);
		SAFE_DELETE(task);
	}
	m_vCommandTable.clear();
}

/* ================================================ */
/**
 * @brief	ボタンが押されたかどうかの判定
 */
/* ================================================ */
bool InputWatcher::IsButtonPush( const BUTTON_KIND &kind )
{
	if( (m_buttonState & kind) && !(m_preButtonState & kind) ){
		return true;
	}
	return false;
}

/* ================================================ */
/**
 * @brief	ボタンが押されているかどうかの判定
 */
/* ================================================ */
bool InputWatcher::IsButtonPress( const BUTTON_KIND &kind )
{
	if(m_buttonState & kind){
		return true;
	}
	return false;
}

/* ================================================ */
/**
 * @brief	ボタンが離されたかどうかの判定
 */
/* ================================================ */
bool InputWatcher::IsButtonRelease( const BUTTON_KIND &kind )
{
	if( !(m_buttonState & kind) && (m_preButtonState & kind) ){
		return true;
	}
	return false;
}

/* ================================================ */
/**
 * @brief	ボタンのイベントステータス設定
 */
/* ================================================ */
void InputWatcher::SetPadButtonState( const BUTTON_KIND &kind, const BUTTON_EVENT_KIND &eventKind )
{
	BUTTON button = ConvButtonKindToButton(kind);
	m_buttonEventStatus[button] = eventKind;
}

/* ================================================ */
/**
 * @brief	ボタンのイベントステータスによって呼ぶ関数変更
 */
/* ================================================ */
bool InputWatcher::IsButtonEvent( const BUTTON_KIND &kind )
{
	BUTTON button = ConvButtonKindToButton(kind);

	switch( m_buttonEventStatus[button] ){
	case EVENT_NOUSE:
		break;
	case EVENT_PUSH:
		return IsButtonPush(kind);
		break;
	case EVENT_PRESS:
		return IsButtonPress(kind);
		break;
	case EVENT_RELEASE:
		return IsButtonRelease(kind);
		break;
	case EVENT_INVALID:
	default:
		DEBUG_ASSERT( 0, "想定外");
		break;
	}
	return false;
}

/* ================================================ */
/**
 * @brief	パッドを見て対応する関数を呼ぶ
 */
/* ================================================ */
void InputWatcher::CallPadEvent()
{
	//! 保持しているボタン更新
	m_preButtonState = m_buttonState;
	m_buttonState = GetJoypadInputState( m_watchPadIndex );
	for(uint32_t i = 0; i < STICK_MAX; ++i){
		m_stickInfo[i] = GetStickInfo(static_cast<STICK_KIND>(i));
	}

	if(IsButtonEvent(BUTTON_UP)){
		PadEventUp();
	}
	if(IsButtonEvent(BUTTON_DOWN)){
		PadEventDown();
	}
	if(IsButtonEvent(BUTTON_LEFT)){
		PadEventLeft();
	}
	if(IsButtonEvent(BUTTON_RIGHT)){
		PadEventRight();
	}
	if(IsButtonEvent(BUTTON_DECIDE)){
		PadEventDecide();
		DEBUG_PRINT("PAD_INPUT_DECIDE\n");
	}
	if(IsButtonEvent(BUTTON_CANCEL)){
		PadEventCancel();
		DEBUG_PRINT("PAD_INPUT_CANCEL\n");
	}
	if(IsButtonEvent(BUTTON_SPECIAL1)){
		PadEventSpecial1();
		DEBUG_PRINT("PAD_INPUT_SP1\n");
	}
	if(IsButtonEvent(BUTTON_SPECIAL2)){
		PadEventSpecial2();
		DEBUG_PRINT("PAD_INPUT_SP2\n");
	}
	if(IsButtonEvent(BUTTON_L1)){
		PadEventL1();
		DEBUG_PRINT("BUTTON_LB\n");
	}
	if(IsButtonEvent(BUTTON_R1)){
		PadEventR1();
		DEBUG_PRINT("BUTTON_RB\n");
	}
	if(IsButtonEvent(BUTTON_L2)){
		PadEventL2();
		DEBUG_PRINT("BUTTON_L2\n");
	}
	if(IsButtonEvent(BUTTON_R2)){
		PadEventR2();
		DEBUG_PRINT("BUTTON_R2\n");
	}
	if(IsButtonEvent(BUTTON_SELECT)){
		PadEventSelect();
		DEBUG_PRINT("BUTTON_BACK\n");
	}
	if(IsButtonEvent(BUTTON_START)){
		PadEventStart();
		DEBUG_PRINT("BUTTON_START\n");
	}
	if(IsButtonEvent(BUTTON_L3)){
		PadEventL3();
		DEBUG_PRINT("BUTTON_L3\n");
	}
	if(IsButtonEvent(BUTTON_R3)){
		PadEventR3();
		DEBUG_PRINT("BUTTON_R3\n");
	}

	//! 設定されたコマンドの確認
	for(uint32_t i = 0;i < m_vCommandTable.size();++i){
		CommandTable* command = m_vCommandTable.at(i);
		if(command->CheckCommand(m_watchPadIndex)){
			//! 入力成功
			PadEventCommand(command->GetCommandKind());
		}
	}
}

/* ================================================ */
/**
 * @brief	スティックの情報取得(他クラス取得用)
 */
/* ================================================ */
const InputWatcher::STICK_INFO &InputWatcher::GetStickInfoRight()
{
	return m_stickInfo[STICK_RIGHT];
}
const InputWatcher::STICK_INFO &InputWatcher::GetStickInfoLeft()
{
	return m_stickInfo[STICK_LEFT];
}

/* ================================================ */
/**
 * @brief	スティックの情報取得(内部用)
 */
/* ================================================ */
const InputWatcher::STICK_INFO InputWatcher::GetStickInfo( const InputWatcher::STICK_KIND &stickKind )
{
	int xx = 0;
	int yy = 0;
	math::Angle angle = 0.0f;

	if(stickKind == STICK_LEFT){
		GetJoypadAnalogInput( &xx, &yy, m_watchPadIndex );
	}
	else{
		GetJoypadAnalogInputRight( &xx, &yy, m_watchPadIndex );
	}

	if(math::Abs(xx) < STICK_INVALID_RANGE
		&& math::Abs(yy) < STICK_INVALID_RANGE){
		// 傾いてはいるが無効範囲
		angle = 0;
		xx = 0;
		yy = 0;
	}
	else{
		angle = math::ArcTan2( static_cast<float>(yy), static_cast<float>(xx));
	}

	STICK_INFO ret;
	ret.Init();
	ret.m_vec = math::Vector2(static_cast<float>(xx),static_cast<float>(yy));
	ret.m_angle = angle;
	return ret;
}

/* ================================================ */
/**
 * @brief	複数のボタンを押す処理を作成
 */
/* ================================================ */
void InputWatcher::MakeCommandTable( CommandTable *table )
{
	m_vCommandTable.push_back(table);
}

CommandTable::CommandTable( const uint8_t &waitTime, const uint32_t &commandKind )
: m_commandStep(0)
, m_waitTime(0)
, m_waitTimeMax(waitTime)
, m_commandKind(commandKind)
, m_prePadState(0)
{
	for(uint32_t i = 0;i < COMMAND_MAX+1;++i){
		for(uint32_t j = 0;j < COMBINATION_MAX;++j){
			m_table[i][j] = INVALID_VALUE;
		}
	}
}

CommandTable::~CommandTable(){
}

/* ================================================ */
/**
 * @brief	コマンドチェック＋次ステップに進む
 */
/* ================================================ */
bool CommandTable::CheckCommand( const uint32_t &watchPad )
{
	bool isCommandFinish = false;

	if( IsStepButtonPress(m_commandStep, watchPad) ){
		//! 入力待機時間をリセットして次ステップへ
		IncStep();
		DEBUG_PRINT("step : %d \n", m_commandStep);
		m_waitTime = 0;
		if(m_table[m_commandStep][0] == INVALID_VALUE){
			//! コマンド入力成功
			Reset();
			DEBUG_PRINT("コマンド入力成功!!\n");
			isCommandFinish = true;
		}
	}
	else{
		++m_waitTime;
		if(m_waitTime >= m_waitTimeMax){
			//! 残念
			Reset();
		}
	}

	//! 次のフレームのため状態保持
	m_prePadState = GetJoypadInputState( watchPad );

	return isCommandFinish;
}

/* ================================================ */
/**
 * @brief	ステップ等リセット
 */
/* ================================================ */
void CommandTable::Reset()
{
	m_waitTime = 0;
	m_commandStep = 0;
}

/* ================================================ */
/**
 * @brief	指定ステップが押されているかどうか
 */
/* ================================================ */
bool CommandTable::IsStepButtonPress( const uint32_t &stepIndex, const uint32_t &watchPad )
{
	bool isStepPress = true;
	for(uint32_t i = 0; i < COMBINATION_MAX; ++i){
		if(m_table[stepIndex][i] == INVALID_VALUE){
			//!ボタンがセットされていないならチェック終了
			break;
		}
		int state = GetJoypadInputState( watchPad );
		if( (state & m_table[stepIndex][i]) && !(m_prePadState & m_table[stepIndex][i]) ){
			isStepPress = true;
		}
		else{
			isStepPress = false;
			break;
		}
	}
	return isStepPress;
}

InputWatcher::BUTTON InputWatcher::ConvButtonKindToButton( const BUTTON_KIND &kind )
{
	switch(kind){
	case BUTTON_UP:			return UP;		break;
	case BUTTON_DOWN:		return DOWN;	break;
	case BUTTON_RIGHT:		return RIGHT;	break;
	case BUTTON_LEFT:		return LEFT;	break;
	case BUTTON_DECIDE:		return DECIDE;	break;
	case BUTTON_CANCEL:		return CANCEL;	break;
	case BUTTON_SPECIAL1:	return SPECIAL1;break;
	case BUTTON_SPECIAL2:	return SPECIAL2;break;
	case BUTTON_R1:			return R1;		break;
	case BUTTON_L1:			return L1;		break;
	case BUTTON_R2:			return R2;		break;
	case BUTTON_L2:			return L2;		break;
	case BUTTON_START:		return START;	break;
	case BUTTON_SELECT:		return SELECT;	break;
	case BUTTON_R3:			return R3;		break;
	case BUTTON_L3:			return L3;		break;

	default:
		DEBUG_ASSERT( 0, "ボタンの種類が想定外");
		break;
	}
	return BUTTON_MAX;
}