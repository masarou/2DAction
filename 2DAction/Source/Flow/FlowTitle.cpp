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
: FlowBase( fileName )
, m_pMenuWindow( NULL )
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
	// ���̑J�ڐ����ɊĎ�
	if( m_pMenuWindow && !m_pMenuWindow->GetNextFlowStr().empty() ){
		StartFade( m_pMenuWindow->GetNextFlowStr().c_str() );
	}
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
, m_selectNo( 0 )
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
		// ���̑J�ڐ悪���܂����̂łȂɂ����Ȃ�
		return;
	}

	CallPadEvent();
	for( uint32_t i = 0; i < SELECT_MAX; ++i){
		// �J�[�\�����������Ă�����A�j���ύX
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
	// ����SE�炷
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
	// �J�[�\��SE�炷
	SoundManager::GetInstance()->PlaySE("Cursor");
	m_selectNo = (m_selectNo+1) % SELECT_MAX;
}
void TitleMenu::PadEventLeft()
{
	// �J�[�\��SE�炷
	SoundManager::GetInstance()->PlaySE("Cursor");
	m_selectNo = (m_selectNo+(SELECT_MAX - 1)) % SELECT_MAX;
}
