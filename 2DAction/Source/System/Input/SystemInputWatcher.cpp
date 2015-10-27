
/* ====================================================================== */
/**
 * @brief  �p�b�h���͊Ď��N���X
 *
 * @note
 *
*/
/* ====================================================================== */

#include "System/Input/SystemInputWatcher.h"
#include "System/SystemFadeManager.h"
#include "Common/Utility/CommonGameUtility.h"

//! �X�e�B�b�N�̖����͈�
#define STICK_INVALID_RANGE 0

//! 
static const uint32_t s_keyBoardButtonArray[] = {
	KEY_INPUT_UP,			//! BUTTON_UP
	KEY_INPUT_DOWN,			//! BUTTON_DOWN
	KEY_INPUT_RIGHT,		//! BUTTON_RIGHT
	KEY_INPUT_LEFT,			//! BUTTON_LEFT
	KEY_INPUT_SPACE,		//! BUTTON_DECIDE
	KEY_INPUT_C,			//! BUTTON_CANCEL
	KEY_INPUT_I,			//! BUTTON_SPECIAL1
	KEY_INPUT_I,			//! BUTTON_SPECIAL2
	KEY_INPUT_LSHIFT,		//! BUTTON_R1
	KEY_INPUT_LSHIFT,		//! BUTTON_L1
	KEY_INPUT_I,			//! BUTTON_R2
	KEY_INPUT_I,			//! BUTTON_L2
	KEY_INPUT_RETURN,		//! BUTTON_START
	KEY_INPUT_I,			//! BUTTON_SELECT
	KEY_INPUT_I,			//! BUTTON_R3
	KEY_INPUT_I,			//! BUTTON_L3
};

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
	
	static_assert(
		   NUMBEROF(m_keyBoardStateChar) == KEY_BOARD_ARRAY_MAX
		&& NUMBEROF(m_preKeyBoardState) == KEY_BOARD_ARRAY_MAX
		&& NUMBEROF(m_keyBoardState) == KEY_BOARD_ARRAY_MAX
		, "�L�[�{�[�h�z��T�C�Y���قȂ�" );

	for(uint32_t i = 0; i < KEY_BOARD_ARRAY_MAX; ++i){
		m_keyBoardStateChar[i] = '0';
		m_preKeyBoardState[i] = false;
		m_keyBoardState[i] = false;
	}
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
 * @brief	�{�^���������ꂽ���ǂ����̔���
 */
/* ================================================ */
bool InputWatcher::IsButtonPush( const BUTTON_KIND &kind )
{
	// �p�b�h�`�F�b�N
	if( (m_buttonState & kind) && !(m_preButtonState & kind) ){
		return true;
	}
	
	// �L�[�{�[�h�`�F�b�N
	BUTTON button = ConvButtonKindToButton(kind);
	uint32_t index = s_keyBoardButtonArray[button];
	if( !m_preKeyBoardState[index] && m_keyBoardState[index] ){
		return true;
	}
	return false;
}

/* ================================================ */
/**
 * @brief	�{�^����������Ă��邩�ǂ����̔���
 */
/* ================================================ */
bool InputWatcher::IsButtonPress( const BUTTON_KIND &kind )
{
	// �p�b�h�`�F�b�N
	if(m_buttonState & kind){
		return true;
	}

	// �L�[�{�[�h�`�F�b�N
	BUTTON button = ConvButtonKindToButton(kind);
	uint32_t index = s_keyBoardButtonArray[button];
	if( m_keyBoardState[index] ){
		return true;
	}
	return false;
}
bool InputWatcher::IsPreFrameButtonPress( const BUTTON_KIND &kind )
{
	// �p�b�h�`�F�b�N
	if(m_preButtonState & kind){
		return true;
	}

	// �L�[�{�[�h�`�F�b�N
	BUTTON button = ConvButtonKindToButton(kind);
	uint32_t index = s_keyBoardButtonArray[button];
	if( m_preKeyBoardState[index] ){
		return true;
	}
	return false;
}

/* ================================================ */
/**
 * @brief	�{�^���������ꂽ���ǂ����̔���
 */
/* ================================================ */
bool InputWatcher::IsButtonRelease( const BUTTON_KIND &kind )
{
	if( !(m_buttonState & kind) && (m_preButtonState & kind) ){
		return true;
	}

	// �L�[�{�[�h�`�F�b�N
	BUTTON button = ConvButtonKindToButton(kind);
	uint32_t index = s_keyBoardButtonArray[button];
	if( m_preKeyBoardState[index] && !m_keyBoardState[index] ){
		return true;
	}
	return false;
}

/* ================================================ */
/**
 * @brief	�{�^���̃C�x���g�X�e�[�^�X�ݒ�
 */
/* ================================================ */
void InputWatcher::SetPadButtonState( const BUTTON_KIND &kind, const BUTTON_EVENT_KIND &eventKind )
{
	BUTTON button = ConvButtonKindToButton(kind);
	m_buttonEventStatus[button] = eventKind;
}

/* ================================================ */
/**
 * @brief	�{�^���̃C�x���g�X�e�[�^�X�ɂ���ČĂԊ֐��ύX
 */
/* ================================================ */
bool InputWatcher::IsButtonEvent( const BUTTON_KIND &kind )
{
	// �Q�[���p�b�h��Ԃ���`�F�b�N
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
		DEBUG_ASSERT( 0, "�z��O");
		break;
	}

	return false;
}

/* ================================================ */
/**
 * @brief	�p�b�h�����đΉ�����֐����Ă�
 */
/* ================================================ */
void InputWatcher::CallPadEvent()
{
	// �t�F�[�h�������Ă��Ȃ��Ȃ瑀�얳��
	if( FadeManager::GetInstance()->GetCurrentState() != FadeManager::STATE_IDLE ){
		return;
	}

	//! �{�^�����X�V
	//! �p�b�h
	m_preButtonState = m_buttonState;
	m_buttonState = GetJoypadInputState( m_watchPadIndex );

	// �p�b�h�A�i���O�X�e�B�b�N���X�V
	for(uint32_t i = 0; i < STICK_MAX; ++i){
		m_stickInfo[i] = GetStickInfo(static_cast<STICK_KIND>(i));
	}

	//! �L�[�{�[�h���X�V
	GetHitKeyStateAll( m_keyBoardStateChar );
	for( uint32_t i = 0; i < KEY_BOARD_ARRAY_MAX ; ++i ){
		m_preKeyBoardState[i] = m_keyBoardState[i];
		m_keyBoardState[i] = ( m_keyBoardStateChar[i] == 0 ) ? 0 : 1 ;
	}
	SetStickInfoFromKeyBoard();

	if(IsButtonEvent(BUTTON_START)){
		PadEventStart();
		DEBUG_PRINT("BUTTON_START\n");
	}

	// Pause���Ȃ�Start�ȊO���얳��
	if( Utility::IsGamePause() ){
		return;
	}

	if(IsButtonEvent(BUTTON_UP)){
		PadEventUp();
		//DEBUG_PRINT("PAD_INPUT_UP\n");
	}
	if(IsButtonEvent(BUTTON_DOWN)){
		PadEventDown();
		//DEBUG_PRINT("PAD_INPUT_DOWN\n");
	}
	if(IsButtonEvent(BUTTON_LEFT)){
		PadEventLeft();
		//DEBUG_PRINT("PAD_INPUT_LEFT\n");
	}
	if(IsButtonEvent(BUTTON_RIGHT)){
		PadEventRight();
		//DEBUG_PRINT("PAD_INPUT_RIGHT\n");
	}
	if(IsButtonEvent(BUTTON_DECIDE)){
		PadEventDecide();
		//DEBUG_PRINT("PAD_INPUT_DECIDE\n");
	}
	if(IsButtonEvent(BUTTON_CANCEL)){
		PadEventCancel();
		//DEBUG_PRINT("PAD_INPUT_CANCEL\n");
	}
	if(IsButtonEvent(BUTTON_SPECIAL1)){
		PadEventSpecial1();
		//DEBUG_PRINT("PAD_INPUT_SP1\n");
	}
	if(IsButtonEvent(BUTTON_SPECIAL2)){
		PadEventSpecial2();
		//DEBUG_PRINT("PAD_INPUT_SP2\n");
	}
	if(IsButtonEvent(BUTTON_L1)){
		PadEventL1();
		//DEBUG_PRINT("BUTTON_LB\n");
	}
	if(IsButtonEvent(BUTTON_R1)){
		PadEventR1();
		//DEBUG_PRINT("BUTTON_RB\n");
	}
	if(IsButtonEvent(BUTTON_L2)){
		PadEventL2();
		//DEBUG_PRINT("BUTTON_L2\n");
	}
	if(IsButtonEvent(BUTTON_R2)){
		PadEventR2();
		//DEBUG_PRINT("BUTTON_R2\n");
	}
	if(IsButtonEvent(BUTTON_SELECT)){
		PadEventSelect();
		//DEBUG_PRINT("BUTTON_BACK\n");
	}
	if(IsButtonEvent(BUTTON_L3)){
		PadEventL3();
		//DEBUG_PRINT("BUTTON_L3\n");
	}
	if(IsButtonEvent(BUTTON_R3)){
		PadEventR3();
		//DEBUG_PRINT("BUTTON_R3\n");
	}

	//! �ݒ肳�ꂽ�R�}���h�̊m�F
	for(uint32_t i = 0;i < m_vCommandTable.size();++i){
		CommandTable* command = m_vCommandTable.at(i);
		if(command->CheckCommand(m_watchPadIndex)){
			//! ���͐���
			PadEventCommand(command->GetCommandKind());
		}
	}
}

/* ================================================ */
/**
 * @brief	�X�e�B�b�N�̏��擾(���N���X�擾�p)
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
 * @brief	�X�e�B�b�N�̏��擾(�����p)
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
		// �X���Ă͂��邪�����͈�
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
 * @brief	�����̃{�^���������������쐬
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
 * @brief	�R�}���h�`�F�b�N�{���X�e�b�v�ɐi��
 */
/* ================================================ */
bool CommandTable::CheckCommand( const uint32_t &watchPad )
{
	bool isCommandFinish = false;

	if( IsStepButtonPress(m_commandStep, watchPad) ){
		//! ���͑ҋ@���Ԃ����Z�b�g���Ď��X�e�b�v��
		IncStep();
		DEBUG_PRINT("step : %d \n", m_commandStep);
		m_waitTime = 0;
		if(m_table[m_commandStep][0] == INVALID_VALUE){
			//! �R�}���h���͐���
			Reset();
			DEBUG_PRINT("�R�}���h���͐���!!\n");
			isCommandFinish = true;
		}
	}
	else{
		++m_waitTime;
		if(m_waitTime >= m_waitTimeMax){
			//! �c�O
			Reset();
		}
	}

	//! ���̃t���[���̂��ߏ�ԕێ�
	m_prePadState = GetJoypadInputState( watchPad );

	return isCommandFinish;
}

/* ================================================ */
/**
 * @brief	�X�e�b�v�����Z�b�g
 */
/* ================================================ */
void CommandTable::Reset()
{
	m_waitTime = 0;
	m_commandStep = 0;
}

/* ================================================ */
/**
 * @brief	�w��X�e�b�v��������Ă��邩�ǂ���
 */
/* ================================================ */
bool CommandTable::IsStepButtonPress( const uint32_t &stepIndex, const uint32_t &watchPad )
{
	bool isStepPress = true;
	for(uint32_t i = 0; i < COMBINATION_MAX; ++i){
		if(m_table[stepIndex][i] == INVALID_VALUE){
			//!�{�^�����Z�b�g����Ă��Ȃ��Ȃ�`�F�b�N�I��
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
		DEBUG_ASSERT( 0, "�{�^���̎�ނ��z��O");
		break;
	}
	return BUTTON_MAX;
}

/* ================================================ */
/**
 * @brief	�L�[�{�[�h�̏�Ԃ���[���I�ɃA�i���O�X�e�B�b�N�p�ϐ��ɐ����Z�b�g
 */
/* ================================================ */
void InputWatcher::SetStickInfoFromKeyBoard()
{
	if( m_stickInfo[STICK_LEFT].m_vec == math::Vector2() ){
		if( m_keyBoardState[KEY_INPUT_UP] && m_keyBoardState[KEY_INPUT_RIGHT] ){
			m_stickInfo[STICK_LEFT].m_vec = math::Vector2( 1000.0f, -1000.0f );
			m_stickInfo[STICK_LEFT].m_angle = 45.0f;
		}
		else if( m_keyBoardState[KEY_INPUT_RIGHT] && m_keyBoardState[KEY_INPUT_DOWN] ){
			m_stickInfo[STICK_LEFT].m_vec = math::Vector2( 1000.0f, 1000.0f );
			m_stickInfo[STICK_LEFT].m_angle = 135.0f;
		}
		else if( m_keyBoardState[KEY_INPUT_DOWN] && m_keyBoardState[KEY_INPUT_LEFT] ){
			m_stickInfo[STICK_LEFT].m_vec = math::Vector2( -1000.0f, 1000.0f );
			m_stickInfo[STICK_LEFT].m_angle = 225.0f;
		}
		else if( m_keyBoardState[KEY_INPUT_LEFT] && m_keyBoardState[KEY_INPUT_UP] ){
			m_stickInfo[STICK_LEFT].m_vec = math::Vector2( -1000.0f, -1000.0f );
			m_stickInfo[STICK_LEFT].m_angle = 315.0f;
		}
		else if( m_keyBoardState[KEY_INPUT_UP] ){
			m_stickInfo[STICK_LEFT].m_vec = math::Vector2( 0.0f, -1000.0f );
			m_stickInfo[STICK_LEFT].m_angle = 90.0f;
		}
		else if( m_keyBoardState[KEY_INPUT_DOWN] ){
			m_stickInfo[STICK_LEFT].m_vec = math::Vector2( 0.0f, 1000.0f );
			m_stickInfo[STICK_LEFT].m_angle = 270.0f;
		}
		else if( m_keyBoardState[KEY_INPUT_RIGHT] ){
			m_stickInfo[STICK_LEFT].m_vec = math::Vector2( 1000.0f, 0.0f );
			m_stickInfo[STICK_LEFT].m_angle = 0.0f;
		}
		else if( m_keyBoardState[KEY_INPUT_LEFT] ){
			m_stickInfo[STICK_LEFT].m_vec = math::Vector2( -1000.0f, 0.0f );
			m_stickInfo[STICK_LEFT].m_angle = 180.0f;
		}
	}
	if( m_stickInfo[STICK_RIGHT].m_vec == math::Vector2() ){
		if( m_keyBoardState[KEY_INPUT_W] && m_keyBoardState[KEY_INPUT_D] ){
			m_stickInfo[STICK_RIGHT].m_vec = math::Vector2( 1000.0f, -1000.0f );
			m_stickInfo[STICK_RIGHT].m_angle = 45.0f;
		}
		else if( m_keyBoardState[KEY_INPUT_D] && m_keyBoardState[KEY_INPUT_S] ){
			m_stickInfo[STICK_RIGHT].m_vec = math::Vector2( 1000.0f, 1000.0f );
			m_stickInfo[STICK_RIGHT].m_angle = 135.0f;
		}
		else if( m_keyBoardState[KEY_INPUT_S] && m_keyBoardState[KEY_INPUT_A] ){
			m_stickInfo[STICK_RIGHT].m_vec = math::Vector2( -1000.0f, 1000.0f );
			m_stickInfo[STICK_RIGHT].m_angle = 225.0f;
		}
		else if( m_keyBoardState[KEY_INPUT_A] && m_keyBoardState[KEY_INPUT_W] ){
			m_stickInfo[STICK_RIGHT].m_vec = math::Vector2( -1000.0f, -1000.0f );
			m_stickInfo[STICK_RIGHT].m_angle = 315.0f;
		}
		else if( m_keyBoardState[KEY_INPUT_W] ){
			m_stickInfo[STICK_RIGHT].m_vec = math::Vector2( 0.0f, -1000.0f );
			m_stickInfo[STICK_RIGHT].m_angle = 90.0f;
		}
		else if( m_keyBoardState[KEY_INPUT_S] ){
			m_stickInfo[STICK_RIGHT].m_vec = math::Vector2( 0.0f, 1000.0f );
			m_stickInfo[STICK_RIGHT].m_angle = 270.0f;
		}
		else if( m_keyBoardState[KEY_INPUT_D] ){
			m_stickInfo[STICK_RIGHT].m_vec = math::Vector2( 1000.0f, 0.0f );
			m_stickInfo[STICK_RIGHT].m_angle = 0.0f;
		}
		else if( m_keyBoardState[KEY_INPUT_A] ){
			m_stickInfo[STICK_RIGHT].m_vec = math::Vector2( -1000.0f, 0.0f );
			m_stickInfo[STICK_RIGHT].m_angle = 180.0f;
		}
	}
}
