/* ====================================================================== */
/**
 * @brief  
 *		�Q�[���J�n�O�A���[�U�[�̊�{�\��UP���
 * @note
 *		
 */
/* ====================================================================== */
#include "FlowStageSelect.h"
#include "Game/GameRecorder.h"
#include "Flow/Process/FlowProcessFirstManual.h"

#include "Common/Utility/CommonGameUtility.h"

FlowBase *FlowStageSelect::Create( const std::string &fileName )
{
	return NEW FlowStageSelect(fileName);
}

FlowStageSelect::FlowStageSelect( const std::string &fileName )
: FlowMenuBase(fileName)
{
	DEBUG_PRINT("FlowStageSelect�����I�I\n");
}


FlowStageSelect::~FlowStageSelect(void)
{
	DEBUG_PRINT("FlowStageSelect�폜�I�I\n");
}

bool FlowStageSelect::Init()
{
	m_pMenuWindow = StageSelectMenu::CreateStageSelectMenu( "MenuStageSelect.json" );
	return true;
}

/* ====================================================================== */
/**
 * @brief  
 *		FlowStageSelect�̕\�������g
 * @note
 *		
 */
/* ====================================================================== */
StageSelectMenu *StageSelectMenu::CreateStageSelectMenu( const std::string &fileName )
{
	return NEW StageSelectMenu( fileName );
}

StageSelectMenu::StageSelectMenu( const std::string &fileName )
: MenuWindow( fileName )
, m_selectStageKind( SEELCT_STAGE01 )
, m_selectStageMax( SEELCT_STAGE02 )
{
	Common::SAVE_DATA saveData;
	Utility::GetSaveData( saveData );
	if( saveData.m_isClearPhase02 ){
		m_selectStageMax = SELECT_MAX;
	}
	else if( saveData.m_isClearPhase01 ){
		m_selectStageMax = SEELCT_STAGE03;
	}
}

StageSelectMenu::~StageSelectMenu(void)
{

}

/* ================================================ */
/**
 * @brief	��ʏ���
 */
/* ================================================ */
bool StageSelectMenu::InitMenu()
{
	// �����L�[�̃p�b�h�C�x���g��PUSH�ŌĂԂ悤�ɐݒ�
	SetPadButtonState( InputWatcher::BUTTON_RIGHT,	InputWatcher::EVENT_PUSH );
	SetPadButtonState( InputWatcher::BUTTON_LEFT,	InputWatcher::EVENT_PUSH );

	// �p�[�c�̏���
	MenuParts *pParts = GetParts( "frame" );
	if( pParts ){
		TEX_DRAW_INFO &drawInfo = pParts->GetTexDrawInfo();
		drawInfo.m_prioity = Common::PRIORITY_ABOVE_NORMAL;
	}
	pParts = GetParts( "stage01" );
	if( pParts ){
		TEX_DRAW_INFO &drawInfo = pParts->GetTexDrawInfo();
		drawInfo.m_prioity = Common::PRIORITY_BELOW_NORMAL;
	}
	pParts = GetParts( "stage02" );
	if( pParts ){
		TEX_DRAW_INFO &drawInfo = pParts->GetTexDrawInfo();
		drawInfo.m_prioity = Common::PRIORITY_BELOW_NORMAL;
	}
	pParts = GetParts( "stage03" );
	if( pParts ){
		TEX_DRAW_INFO &drawInfo = pParts->GetTexDrawInfo();
		drawInfo.m_prioity = Common::PRIORITY_BELOW_NORMAL;
	}

	// ��ʕ`��X�V
	ChangeDispState( m_selectStageKind );

	return true;
}

/* ================================================ */
/**
 * @brief	���S�O����
 */
/* ================================================ */
bool StageSelectMenu::DieMainMenu()
{
	return true;
}

/* ================================================ */
/**
 * @brief	�`�悷�鋭�����ڐ����X�V
 */
/* ================================================ */
void StageSelectMenu::ChangeDispState( const SELECT_STAGE &stage )
{
	MenuParts *pParts = NULL;

	std::string partsName = "stage0";

	// ��������w�i��S��\��
	for( uint32_t i = 0; i < SELECT_MAX ; ++i ){
		std::string partsStr = partsName;
		partsStr += '1' + i;
		pParts = GetParts( partsStr.c_str() );
		if( pParts ){
			pParts->SetDrawFlag( false );
		}
	}

	std::string dispPartsStr = partsName;
	dispPartsStr += '1' + static_cast<uint32_t>(stage);
	pParts = GetParts( dispPartsStr.c_str() );
	if( pParts ){
		pParts->SetDrawFlag( true );
	}

	// �������X�V
	m_explanationStr = GetExplanationStr( stage );
}

void StageSelectMenu::UpdateMenu()
{
	CallPadEvent();
	
	// �������Z�b�g
	Draw2DManager::GetInstance()->PushDrawString( m_explanationStr, math::Vector2( 270.0f, 480.0f ) );
}

/* ================================================ */
/**
 * @brief	�p�b�h����֐�
 */
/* ================================================ */
void StageSelectMenu::PadEventDecide()
{
	// ����SE�炷
	SoundManager::GetInstance()->PlaySE("Decide");

	switch( m_selectStageKind ){
	default:
		DEBUG_ASSERT( 0, "�z��O�̒l");
	case SEELCT_STAGE01:
		SetNextFlowStr( "start01" );
		break;
	case SEELCT_STAGE02:
		SetNextFlowStr( "start02" );
		// �r������v���C���ǂ����t���O�Z�b�g(�����L���O���f�Ȃ�)
		GameRecorder::GetInstance()->SetContinueFlag();
		break;
	case SEELCT_STAGE03:
		// �r������v���C���ǂ����t���O�Z�b�g(�����L���O���f�Ȃ�)
		GameRecorder::GetInstance()->SetContinueFlag();
		SetNextFlowStr( "start03" );
		break;
	}
}

void StageSelectMenu::PadEventRight()
{
	// �J�[�\��SE�炷
	SoundManager::GetInstance()->PlaySE("Cursor");

	uint32_t selectNo = ( static_cast<uint32_t>(m_selectStageKind) + 1 ) % m_selectStageMax;
	m_selectStageKind = static_cast<SELECT_STAGE>(selectNo);
	ChangeDispState( m_selectStageKind );
}

void StageSelectMenu::PadEventLeft()
{
	// �J�[�\��SE�炷
	SoundManager::GetInstance()->PlaySE("Cursor");

	uint32_t selectNo = ( static_cast<uint32_t>(m_selectStageKind) + (m_selectStageMax - 1) ) % m_selectStageMax;
	m_selectStageKind = static_cast<SELECT_STAGE>(selectNo);
	ChangeDispState( m_selectStageKind );
}

/* ================================================ */
/**
 * @brief	���ڂ��Ƃ̐������擾
 */
/* ================================================ */
std::string StageSelectMenu::GetExplanationStr( const SELECT_STAGE &stage )
{
	std::string retStr = "";
	switch( stage ){
	case SEELCT_STAGE01:
		retStr = "Stage1-1����n�߂܂��B\n�ʏ�̃v���C";
		break;

	case SEELCT_STAGE02:
		retStr = "Stage2-1����n�߂܂��B";
		break;

	case SEELCT_STAGE03:
		retStr = "Stage3-1����n�߂܂��B";
		break;
	}
	return retStr;
}
