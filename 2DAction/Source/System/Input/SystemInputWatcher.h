/* ====================================================================== */
/**
 * @brief  �p�b�h���͊Ď��N���X
 *
 * @note
 *		�g�p����ꍇ�͂��̃N���X���p���̏�A���肪�~��������
 *		CallPadEvent()���Ă�ł��������B�{�^���ɑΉ������֐����Ă΂�܂��B
 *		
 *		�������͂̑����MakeCommandTable(CommandTable)���g�p�B
 *		���������PadEventCommand���Ă΂��B
 *		�]�Ɏ���30�e�̃R�i�~�R�}���h
 *				CommandTable *table = NEW CommandTable(30, 7);
 *				table->m_table[0][0] = BUTTON_UP;
 *				table->m_table[1][0] = BUTTON_UP;
 *				table->m_table[2][0] = BUTTON_DOWN;
 *				table->m_table[3][0] = BUTTON_DOWN;
 *				table->m_table[4][0] = BUTTON_LEFT;
 *				table->m_table[5][0] = BUTTON_RIGHT;
 *				table->m_table[6][0] = BUTTON_LEFT;
 *				table->m_table[7][0] = BUTTON_RIGHT;
 *				table->m_table[8][0] = BUTTON_DECIDE;
 *				table->m_table[9][0] = BUTTON_CANCEL;
 *				MakeCommandTable(table);
*/
/* ====================================================================== */

#ifndef __SYSTEM_INPUT_WATCHER__
#define __SYSTEM_INPUT_WATCHER__

#include "System/SystemDefine.h"
#include "System/Task/SystemTaskUnit.h"
#include "Math/MathCommon.h"
#include "Math/MathVector.h"
#include "Math/MathAngle.h"

class CommandTable;

class InputWatcher
{

public:

	//! ���ɂ���enum BUTTON�Ɛ������킹�邱��
	enum BUTTON_KIND{
		BUTTON_UP		= PAD_INPUT_UP,
		BUTTON_DOWN		= PAD_INPUT_DOWN,
		BUTTON_RIGHT	= PAD_INPUT_RIGHT,
		BUTTON_LEFT		= PAD_INPUT_LEFT,
		BUTTON_DECIDE	= PAD_INPUT_1,
		BUTTON_CANCEL	= PAD_INPUT_2,
		BUTTON_SPECIAL1	= PAD_INPUT_3,
		BUTTON_SPECIAL2	= PAD_INPUT_4,
		BUTTON_R1		= PAD_INPUT_6,
		BUTTON_L1		= PAD_INPUT_5,
		BUTTON_R2		= PAD_INPUT_12,
		BUTTON_L2		= PAD_INPUT_11,
		BUTTON_START	= PAD_INPUT_8,
		BUTTON_SELECT	= PAD_INPUT_7,
		BUTTON_R3		= PAD_INPUT_10,
		BUTTON_L3		= PAD_INPUT_9,

		BUTTON_INVALID	= INVALID_VALUE,
	};

	enum BUTTON_EVENT_KIND{
		EVENT_NOUSE,
		EVENT_PUSH,
		EVENT_PRESS,
		EVENT_RELEASE,

		EVENT_INVALID,
	};

	enum STICK_KIND{
		STICK_RIGHT,
		STICK_LEFT,

		STICK_MAX,
	};

	struct STICK_INFO{
		math::Vector2 m_vec;
		math::Angle m_angle;
		void Init(){
			m_vec = math::Vector2(0.0f,0.0f);
			m_angle = 0.0f;
		}
	};

	// �L�[�{�[�h�z��ő吔
	static const uint32_t KEY_BOARD_ARRAY_MAX = 256;

protected:

	InputWatcher( const uint32_t &padIndex = DX_INPUT_PAD1 );
	~InputWatcher(void);
	
	//! �Ď��p�b�h�ύX
	void ChangeWatchPadIndex( const uint32_t &padIndex ){m_watchPadIndex = padIndex;}

	//! �{�^����������
	const STICK_INFO &GetStickInfoRight();
	const STICK_INFO &GetStickInfoLeft();
	bool IsButtonPush( const BUTTON_KIND &kind );
	bool IsButtonPress( const BUTTON_KIND &kind );
	bool IsButtonRelease( const BUTTON_KIND &kind );
	bool IsPreFrameButtonPress( const BUTTON_KIND &kind );

	//!�{�^���C�x���g���ǂ̃^�C�~���O�ŌĂԂ��ݒ�
	void SetPadButtonState( const BUTTON_KIND &kind, const BUTTON_EVENT_KIND &eventKind );

	//! �{�^���C�x���g�`�F�b�N
	bool IsButtonEvent( const BUTTON_KIND &kind );

	void CallPadEvent();
	virtual void PadEventDecide(){};
	virtual void PadEventCancel(){};
	virtual void PadEventSpecial1(){};
	virtual void PadEventSpecial2(){};
	virtual void PadEventL1(){};
	virtual void PadEventL2(){};
	virtual void PadEventL3(){};
	virtual void PadEventR1(){};
	virtual void PadEventR2(){};
	virtual void PadEventR3(){};
	virtual void PadEventStart(){};
	virtual void PadEventSelect(){};
	virtual void PadEventUp(){};
	virtual void PadEventDown(){};
	virtual void PadEventRight(){};
	virtual void PadEventLeft(){};
	virtual void PadEventCommand( const uint32_t &commandKind ){};

	//! �ȉ��K�v�Ȏ��ɌĂ�
	//! �R�}���h�쐬
	void MakeCommandTable( CommandTable *table );

private:

	//! ���������ŕK�v�Ȏ��(BUTTON_KIND�Ɛ������킹�邱��)
	enum BUTTON{
		UP,
		DOWN,
		RIGHT,
		LEFT,
		DECIDE,
		CANCEL,
		SPECIAL1,
		SPECIAL2,
		R1,
		L1,
		R2,
		L2,
		START,
		SELECT,
		R3,
		L3,

		BUTTON_MAX,
	};

	const STICK_INFO GetStickInfo( const STICK_KIND &stickKind );
	BUTTON	ConvButtonKindToButton( const BUTTON_KIND &kind );
	void	SetStickInfoFromKeyBoard();					//!< ���݂̃L�[�{�[�h�̏�Ԃ���[���I�ɃA�i���O�X�e�B�b�N�ɏ��Z�b�g

	uint32_t	m_buttonState;							//!< ���݂̃{�^���X�e�[�^�X
	uint32_t	m_preButtonState;						//!< �O�t���[���̃{�^���X�e�[�^�X
	STICK_INFO	m_stickInfo[STICK_MAX];					//!< �X�e�B�b�N�̃X�e�[�^�X
	char		m_keyBoardStateChar[KEY_BOARD_ARRAY_MAX];//!< ���݂̃L�[�{�[�h�X�e�[�^�X

	//! �{�^���C�x���g���ǂ̃^�C�~���O�ŌĂԁA�Ă΂Ȃ��̎���
	BUTTON_EVENT_KIND	m_buttonEventStatus[BUTTON_MAX];

	//! �R�}���h�e�[�u��
	std::vector<CommandTable*>	m_vCommandTable;

	//! ���̃N���X�����Ă���p�b�h
	uint32_t					m_watchPadIndex;

	//! �L�[�{�[�h�̃{�^�������t���O
	bool						m_preKeyBoardState[KEY_BOARD_ARRAY_MAX];
	bool						m_keyBoardState[KEY_BOARD_ARRAY_MAX];
};


/* ================================================ */
/**
 * @brief	�R�}���h�Ǘ��N���X(�R�}���h1��)
 */
/* ================================================ */
class CommandTable{

public:

	CommandTable( const uint8_t &waitTime, const uint32_t &messageKind );
	~CommandTable();
	static const uint8_t COMBINATION_MAX = 3;
	static const uint8_t COMMAND_MAX = 10;

	//! �R�}���h�̎�ގ擾
	const uint32_t &GetCommandKind(){return m_commandKind;}

	//! �R�}���h�`�F�b�N�{���X�e�b�v�ɐi��
	bool CheckCommand( const uint32_t &watchPad );
	//! ���ۂ̃R�}���h
	uint32_t m_table[COMMAND_MAX+1][COMBINATION_MAX];

private:

	//! ���̃X�e�b�v��
	void IncStep(){++m_commandStep;}
	//! �X�e�b�v�����Z�b�g
	void Reset();
	//! �w��X�e�b�v��������Ă��邩�ǂ���
	bool IsStepButtonPress( const uint32_t &stepIndex, const uint32_t &watchPad );

	//! ���݂ǂ��܂ŃR�}���h���͂�����Ă��邩
	uint8_t m_commandStep;
	//! �R�}���h���̗͂]�Ɏ���
	uint8_t m_waitTime;
	uint8_t m_waitTimeMax;
	//! �R�}���h�������������ɓ����郁�b�Z�[�W
	uint32_t m_commandKind;

	uint32_t m_prePadState;
};

#endif//_SYSTEM_INPUT_WATCHER_

