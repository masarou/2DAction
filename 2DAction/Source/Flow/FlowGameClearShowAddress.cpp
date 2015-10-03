/* ====================================================================== */
/**
 * @brief  
 *		�Q�[���N���A�[�ナ���N�\�����
 * @note
 *		
 */
/* ====================================================================== */
#include "FlowGameClearShowAddress.h"


FlowBase *FlowGameClear::Create( const std::string &fileName )
{
	return NEW FlowGameClear(fileName);
}

FlowGameClear::FlowGameClear( const std::string &fileName )
: FlowMenuBase(fileName)
{
	DEBUG_PRINT("FlowGameClear�����I�I\n");
}


FlowGameClear::~FlowGameClear(void)
{
	DEBUG_PRINT("FlowResult�폜�I�I\n");
}

bool FlowGameClear::Init()
{
	// �w�i�ꖇ�G�쐬
	m_pMenuWindow = ShowAddressMenu::CreateViewScore2D( "GameClearShowAddress.json" );
	return true;
}


ShowAddressMenu *ShowAddressMenu::CreateViewScore2D( const std::string &fileName )
{
	return NEW ShowAddressMenu(fileName);
}

ShowAddressMenu::ShowAddressMenu( const std::string &fileName )
	: MenuWindow(fileName, Common::PRIORITY_NORMAL, WINDOW_CENTER )
{
	DEBUG_PRINT("ShowAddressMenu�����I�I\n");
}


ShowAddressMenu::~ShowAddressMenu(void)
{
	DEBUG_PRINT("ShowAddressMenu�폜�I�I\n");
}

void ShowAddressMenu::UpdateMenu()
{
	if( !GetNextFlowStr().empty() ){
		// ���̑J�ڐ悪���܂����̂łȂɂ����Ȃ�
		return;
	}

	CallPadEvent();
}

/* ================================================ */
/**
 * @brief	�p�b�h����֐�
 */
/* ================================================ */
void ShowAddressMenu::PadEventDecide()
{
	// �L�����Z��SE�炷
	SoundManager::GetInstance()->PlaySE("Cancel");
	SetNextFlowStr( "title" );
}


