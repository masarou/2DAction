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
: FlowBase(fileName)
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
	// ���̑J�ڐ����ɊĎ�
	if( m_pMenuWindow && !m_pMenuWindow->GetNextFlowStr().empty() ){
		StartFade( m_pMenuWindow->GetNextFlowStr().c_str() );
	}
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
, m_selectNo( 0 )
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

	// �I�������ڂ̃Z�b�g�A�b�v
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
			DEBUG_ASSERT( 0, "�I�����ԍ����z��O");
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
	// ����SE�炷
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
				DEBUG_ASSERT( 0, "�z��O�̃t���[" );
				// �Ƃ肠�����^�C�g����
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
	// �J�[�\��SE�炷
	SoundManager::GetInstance()->PlaySE("Cursor");
	m_selectNo = (m_selectNo+1) % SELECT_RETRY_MAX;
}
void IntervalMenu::PadEventLeft()
{
	// �J�[�\��SE�炷
	SoundManager::GetInstance()->PlaySE("Cursor");
	m_selectNo = (m_selectNo+(SELECT_RETRY_MAX - 1)) % SELECT_RETRY_MAX;
}

void IntervalMenu::UpdateMenu()
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
