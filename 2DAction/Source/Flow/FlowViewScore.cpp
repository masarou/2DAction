/* ====================================================================== */
/**
 * @brief  
 *		�Q�[������\���N���X
 * @note
 *		
 */
/* ====================================================================== */

#include "FlowViewScore.h"
#include "Common/Utility/CommonGameUtility.h"

FlowBase *FlowViewScore::Create( const std::string &fileName )
{
	return NEW FlowViewScore(fileName);
}

FlowViewScore::FlowViewScore( const std::string &fileName )
: FlowMenuBase(fileName)
{
	DEBUG_PRINT("FlowViewScore�����I�I\n");
}


FlowViewScore::~FlowViewScore(void)
{
	DEBUG_PRINT("FlowResult�폜�I�I\n");
}

bool FlowViewScore::Init()
{
	// �w�i�ꖇ�G�쐬
	m_pMenuWindow = ViewScoreMenu::CreateViewScore2D( "MenuViewScore.json" );
	return true;
}

/* ====================================================================== */
/**
 * @brief  
 *		�\�����鐔���J�E���^
 * @note
 *		
 */
/* ====================================================================== */
ViewScoreMenu *ViewScoreMenu::CreateViewScore2D( const std::string &readMenuJson )
{
	return NEW ViewScoreMenu( readMenuJson );
}

ViewScoreMenu::ViewScoreMenu( const std::string &readMenuJson )
: MenuWindow( readMenuJson )
{
}

ViewScoreMenu::~ViewScoreMenu(void)
{
}

bool ViewScoreMenu::InitMenu()
{
	// �L�����Z���{�^���L����
	SetPadButtonState( InputWatcher::BUTTON_DOWN,	InputWatcher::EVENT_PUSH );

	// �����L���O�擾
	Utility::GetSaveData( m_saveData );

	// �����L���O�`��
	for( uint32_t i = 0; i < Common::RANKING_RECORD_MAX; ++i ){
		std::string partsStr = "rank";
		partsStr += '0' + i;
		PartsCounter *pCounter = GetPartsCounter( partsStr );
		if( pCounter ){
			pCounter->AddValue( m_saveData.m_scoreRanking[i] );
		}
	}

	return true;
}


void ViewScoreMenu::UpdateMenu()
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
void ViewScoreMenu::PadEventCancel()
{
	// �L�����Z��SE�炷
	SoundManager::GetInstance()->PlaySE("Cancel");
	SetNextFlowStr( "return" );
}