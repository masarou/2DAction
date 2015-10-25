/* ====================================================================== */
/**
 * @brief  
 *		�Q�[���N���A�[�ナ���N�\�����
 * @note
 *		
 */
/* ====================================================================== */

#include "Game/Effect/GameEffect.h"
#include "FlowGameClearShowAddress.h"
#include "Common/Utility/CommonGameUtility.h"


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

	// ����SE
	SoundManager::GetInstance()->PlaySE("Cheer");
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

	if( Utility::GetRandamValue( 30, 0 ) == 0 ){
		// �ԉ΃G�t�F�N�g
		math::Vector2 effectPos = math::Vector2( static_cast<float>( Utility::GetRandamValue( WINDOW_WIDTH - 100, 100 ) ), static_cast<float>( Utility::GetRandamValue( WINDOW_HEIGHT - 100, 100 ) ) );
		GameEffect::CreateEffect( GameEffect::EFFECT_FIRE_FLOWER, effectPos );
		SoundManager::GetInstance()->PlaySE("FireFlower");
		DEBUG_PRINT( "FireFlower x = %lf, y = %lf\n", effectPos.x, effectPos.y );
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


