/* ====================================================================== */
/**
 * @brief  
 *		�Q�[���I���ナ�g���C�A���ɐi�ނ��m�F������
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
	DEBUG_PRINT("FlowInterval�����I�I\n");
}


FlowInterval::~FlowInterval(void)
{
	DEBUG_PRINT("FlowInterval�폜�I�I\n");
}

bool FlowInterval::Init()
{
	// �ꖇ�G�쐬
	m_pMenuWindow = IntervalMenu::CreateIntervalMenu( "MenuInterval.json" );
	return true;
}

/* ================================================ */
/**
 * @brief	���ɐi�ނׂ����x���擾�֐�
 */
/* ================================================ */
void FlowInterval::UpdateFlowAfterChildTask()
{
}


/* ====================================================================== */
/**
 * @brief  
 *		FlowInterval�̕\�������g
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
	// �����L�[�̃p�b�h�C�x���g��PUSH�ŌĂԂ悤�ɐݒ�
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
	// ����SE�炷
	SoundManager::GetInstance()->PlaySE("Decide");

	if( GetSelectedNo() == IntervalMenu::SELECT_TITLE ){
		SetNextFlowStr( "title" );
	}
	else{
		switch( FlowManager::GetInstance()->GetCurrentFlowKind() ){
		default:
			DEBUG_ASSERT( 0, "�z��O�̃t���[" );
			// �Ƃ肠�����^�C�g����
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
	// �J�[�\��SE�炷
	SoundManager::GetInstance()->PlaySE("Cursor");
	
	// �I�����Ă���Index�X�V
	uint32_t selectNo = ( GetSelectedNo() + 1 ) % SELECT_RETRY_MAX;
	SetSelectNum( selectNo );

	// �I�����`��X�V
	SetChoiceSelect( GetSelectedNo() );
}
void IntervalMenu::PadEventLeft()
{
	// �J�[�\��SE�炷
	SoundManager::GetInstance()->PlaySE("Cursor");

	// �I�����Ă���Index�X�V
	uint32_t selectNo = ( GetSelectedNo() + (SELECT_RETRY_MAX - 1) ) % SELECT_MAX;
	SetSelectNum( selectNo );

	// �I�����`��X�V
	SetChoiceSelect( GetSelectedNo() );
}

void IntervalMenu::UpdateMenu()
{
	if( !GetNextFlowStr().empty() ){
		// ���̑J�ڐ悪���܂����̂łȂɂ����Ȃ�
		return;
	}

	CallPadEvent();
}

void IntervalMenu::SetChoiceSelect( uint32_t choiceIndex )
{
	for( uint32_t i = 0; i < SELECT_MAX; ++i){
		// �J�[�\�����������Ă�����A�j���ύX
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
