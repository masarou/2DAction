/* ====================================================================== */
/**
 * @brief  
 *		�^�C�g����ʊǗ��N���X
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
	DEBUG_PRINT("FlowTitle�����I�I\n");
}


FlowTitle::~FlowTitle(void)
{
	DEBUG_PRINT("FlowTitle�폜�I�I\n");
}

bool FlowTitle::Init()
{
	// �Q�[���X�R�A��������
	Utility::GameInitALL();

	// �I�����Ǘ��N���X
	m_pMenuWindow = TitleMenu::Create( "MenuTitle.json" );

	// BGM�Đ��J�n
	SoundManager::GetInstance()->PlayBGM( "title" );

	return true;
}

void FlowTitle::UpdateFlowAfterChildTask()
{

}

/* ====================================================================== */
/**
 * @brief  
 *		�^�C�g���I�����N���X
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
	// �����L�[�̃p�b�h�C�x���g��PUSH�ŌĂԂ悤�ɐݒ�
	SetPadButtonState( InputWatcher::BUTTON_UP,		InputWatcher::EVENT_PUSH );
	SetPadButtonState( InputWatcher::BUTTON_DOWN,	InputWatcher::EVENT_PUSH );
	SetPadButtonState( InputWatcher::BUTTON_RIGHT,	InputWatcher::EVENT_PUSH );
	SetPadButtonState( InputWatcher::BUTTON_LEFT,	InputWatcher::EVENT_PUSH );

	// �I�������ڂ̃Z�b�g�A�b�v
	SetChoiceSelect( SELECT_START );
	return true;
}

void TitleMenu::UpdateMenu()
{
	if( !GetNextFlowStr().empty() ){
		// ���̑J�ڐ悪���܂����̂łȂɂ����Ȃ�
		return;
	}

	CallPadEvent();
}


void TitleMenu::SetChoiceSelect( uint32_t choiceIndex )
{
	for( uint32_t i = 0; i < SELECT_MAX; ++i){
		// �J�[�\�����������Ă�����A�j���ύX
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
	// ����SE�炷
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
	// �J�[�\��SE�炷
	SoundManager::GetInstance()->PlaySE("Cursor");
	
	// �I�����Ă���Index�X�V
	uint32_t selectNo = ( GetSelectedNo() + (SELECT_MAX - 1) ) % SELECT_MAX;
	SetSelectNum( selectNo );
	
	// choice�X�V
	SetChoiceSelect( GetSelectedNo() );
}

void TitleMenu::PadEventDown()
{
	// �J�[�\��SE�炷
	SoundManager::GetInstance()->PlaySE("Cursor");
	
	// �I�����Ă���Index�X�V
	uint32_t selectNo = ( GetSelectedNo() + 1 ) % SELECT_MAX;
	SetSelectNum( selectNo );
	
	// choice�X�V
	SetChoiceSelect( GetSelectedNo() );
}

void TitleMenu::PadEventRight()
{

}
void TitleMenu::PadEventLeft()
{
}
