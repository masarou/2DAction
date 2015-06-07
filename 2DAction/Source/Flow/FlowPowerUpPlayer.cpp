/* ====================================================================== */
/**
 * @brief  
 *		�Q�[���J�n�O�A���[�U�[�̊�{�\��UP���
 * @note
 *		
 */
/* ====================================================================== */
#include "FlowPowerUpPlayer.h"
#include "Game/GameRecorder.h"

#include "Common/Utility/CommonGameUtility.h"

FlowBase *FlowPowerUpPlayer::Create( const std::string &fileName )
{
	return NEW FlowPowerUpPlayer(fileName);
}

FlowPowerUpPlayer::FlowPowerUpPlayer( const std::string &fileName )
: FlowBase(fileName)
{
	DEBUG_PRINT("FlowPowerUpPlayer�����I�I\n");
}


FlowPowerUpPlayer::~FlowPowerUpPlayer(void)
{
	DEBUG_PRINT("FlowPowerUpPlayer�폜�I�I\n");
}

bool FlowPowerUpPlayer::Init()
{
	m_pMenuWindow = PowerUpMenu::CreatePowerUp2D( "MenuPowerUpPlayer.json" );
	return true;
}


void FlowPowerUpPlayer::UpdateFlowPreChildTask()
{
	CallPadEvent();
}

/* ================================================ */
/**
 * @brief	�p�b�h����֐�
 */
/* ================================================ */
void FlowPowerUpPlayer::PadEventDecide()
{
	// ����SE�炷
	SoundManager::GetInstance()->PlaySE("Decide");
	StartFade( "proceed" );
}

/* ====================================================================== */
/**
 * @brief  
 *		FlowPowerUpPlayer�̕\�������g
 * @note
 *		
 */
/* ====================================================================== */
PowerUpMenu *PowerUpMenu::CreatePowerUp2D( const std::string &fileName )
{
	return NEW PowerUpMenu( fileName );
}

PowerUpMenu::PowerUpMenu( const std::string &fileName )
: MenuWindow( fileName )
{

}

PowerUpMenu::~PowerUpMenu(void)
{

}


// �`�悷�鋭�����ڐ����X�V
void PowerUpMenu::ChangeDispState( const Common::PLAYER_BASE_STATE &kind )
{
	// ���ډ摜�\��
	// auto texInfo = m_partsMap.find( "itemTex" );

	// ���݃��x���Z�b�g

	// ����Lv�܂ł̃|�C���g�Z�b�g

	// �������Z�b�g


	// �����|�C���g�Z�b�g
}

bool PowerUpMenu::InitMenu()
{
	// �����L�[�̃p�b�h�C�x���g��PUSH�ŌĂԂ悤�ɐݒ�
	SetPadButtonState( InputWatcher::BUTTON_UP,		InputWatcher::EVENT_PUSH );
	SetPadButtonState( InputWatcher::BUTTON_DOWN,	InputWatcher::EVENT_PUSH );
	SetPadButtonState( InputWatcher::BUTTON_RIGHT,	InputWatcher::EVENT_PUSH );
	SetPadButtonState( InputWatcher::BUTTON_LEFT,	InputWatcher::EVENT_PUSH );

	return true;
}

void PowerUpMenu::UpdateMenu()
{
	
}
