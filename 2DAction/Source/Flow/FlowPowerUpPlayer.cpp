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
#include "Flow/Process/FlowProcessFirstManual.h"

#include "Common/Utility/CommonGameUtility.h"

#define KONAMI_COMMAND_SUCCESS 573

FlowBase *FlowPowerUpPlayer::Create( const std::string &fileName )
{
	return NEW FlowPowerUpPlayer(fileName);
}

FlowPowerUpPlayer::FlowPowerUpPlayer( const std::string &fileName )
: FlowMenuBase(fileName)
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

	Common::SAVE_DATA saveData;
	Utility::GetSaveData( saveData );

	if( saveData.m_isFirst ){
		// �ŏ��̐���
		FirstManual *pFirstManual = FirstManual::Create( FirstManual::KIND_POWERUP1 );
		PushStageEffect( pFirstManual );
		pFirstManual = FirstManual::Create( FirstManual::KIND_POWERUP2 );
		PushStageEffect( pFirstManual );
		pFirstManual = FirstManual::Create( FirstManual::KIND_POWERUP3 );
		PushStageEffect( pFirstManual );
		pFirstManual = FirstManual::Create( FirstManual::KIND_POWERUP4 );
		PushStageEffect( pFirstManual );
		pFirstManual = FirstManual::Create( FirstManual::KIND_POWERUP5 );
		PushStageEffect( pFirstManual );
		pFirstManual = FirstManual::Create( FirstManual::KIND_POWERUP6 );
		PushStageEffect( pFirstManual );
	}

	return true;
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
, m_isProceedEnd( false )
, m_selectStateKind( Common::BASE_STATE_LIFE )
, m_explanationStr("")
{
	// �Z�[�u�f�[�^���[�h
	Utility::GetSaveData( m_loadData );
}

PowerUpMenu::~PowerUpMenu(void)
{

}

/* ================================================ */
/**
 * @brief	��ʏ���
 */
/* ================================================ */
bool PowerUpMenu::InitMenu()
{
	// �����L�[�̃p�b�h�C�x���g��PUSH�ŌĂԂ悤�ɐݒ�
	SetPadButtonState( InputWatcher::BUTTON_UP,		InputWatcher::EVENT_PUSH );
	SetPadButtonState( InputWatcher::BUTTON_DOWN,	InputWatcher::EVENT_PUSH );
	SetPadButtonState( InputWatcher::BUTTON_RIGHT,	InputWatcher::EVENT_PUSH );
	SetPadButtonState( InputWatcher::BUTTON_LEFT,	InputWatcher::EVENT_PUSH );

	// �p�[�c�̏���
	MenuParts *pParts = GetParts( "frame" );
	if( pParts ){
		// �O�g�͕\���D��x��Ⴍ����
		TEX_DRAW_INFO &drawInfo = pParts->GetTexDrawInfo();
		drawInfo.m_prioity = Common::PRIORITY_LOW;
	}

	pParts = GetParts( "decisionUp" );
	if( pParts ){
		TEX_DRAW_INFO &drawInfo = pParts->GetTexDrawInfo();
		drawInfo.m_prioity = Common::PRIORITY_LOW;
	}

	// ��ʕ`��X�V
	ChangeDispState( m_selectStateKind );
	UpdateCursorMove();

	{
		CommandTable *table = NEW CommandTable(20, KONAMI_COMMAND_SUCCESS);
		table->m_table[0][0] = BUTTON_UP;
		table->m_table[1][0] = BUTTON_UP;
		table->m_table[2][0] = BUTTON_DOWN;
		table->m_table[3][0] = BUTTON_DOWN;
		table->m_table[4][0] = BUTTON_LEFT;
		table->m_table[5][0] = BUTTON_RIGHT;
		table->m_table[6][0] = BUTTON_LEFT;
		table->m_table[7][0] = BUTTON_RIGHT;
		table->m_table[8][0] = BUTTON_CANCEL;
		table->m_table[9][0] = BUTTON_DECIDE;
		MakeCommandTable(table);
	}

	return true;
}

/* ================================================ */
/**
 * @brief	���S�O����
 */
/* ================================================ */
bool PowerUpMenu::DieMainMenu()
{
	if( m_isProceedEnd ){
		// �]�����|�C���g�͂Ȃ���
		m_loadData.m_battlePoint = 0;
	}

	// ����I��
	m_loadData.m_isFirst = false;

	// �����ōs�����p���[�A�b�v���Z�[�u�f�[�^�ɔ��f
	Utility::OverWriteSaveData( m_loadData );
	return true;
}

/* ================================================ */
/**
 * @brief	�`�悷�鋭�����ڐ����X�V
 */
/* ================================================ */
void PowerUpMenu::ChangeDispState( const Common::PLAYER_BASE_STATE &kind )
{
	std::string	setAnimStr		= "";	// �Z�b�g����摜�A�j��������
	uint32_t	currentLv		= m_loadData.m_playerBaseStateLv[kind];	// ���݂�Lv
	uint32_t	pointToNextLv	= GetPointToNextLevel( kind, currentLv );	// ����Lv�܂ł̕K�v�|�C���g
	
	switch( kind ){
	case Common::BASE_STATE_LIFE:		// ���C�t�̍ő�l�����߂�
		setAnimStr = "life";
		break;

	case Common::BASE_STATE_MOVE_SPEED:	// �_�b�V��(�ړ����x)����
		setAnimStr = "movespd";
		break;

	case Common::BASE_STATE_DEFFENCE:	// ��_���[�W�����߂�
		setAnimStr = "defence";
		break;

	case Common::BASE_STATE_BULLET_SPD:	// �}�V���K���̊Ԋu
		setAnimStr = "bulletspd";
		break;

	case Common::BASE_STATE_BULLET_DMG:	// �}�V���K���̃_���[�W
		setAnimStr = "bulletdamage";
		break;
	
	case Common::BASE_STATE_BLADE_LEVEL:// �a���̃_���[�W
		setAnimStr = "bladelv";
		break;
	
	case Common::BASE_STATE_CONTINUE:	// �R���e�B�j���[
		setAnimStr = "continue";
		break;
	}

	// ���ډ摜�\��
	SetAnim( "itemImage", setAnimStr );

	// �\���ӏ��̕`��
	for( uint32_t i = 0; i < Common::BASE_STATE_MAX ; ++i ){
		std::string animStr = "idle";
		if( i == static_cast<uint32_t>(kind) ){
			animStr = "select";
		}
		std::string pointPartsStr = "point";
		pointPartsStr += '0' + i;
		SetAnim( pointPartsStr, animStr );
	}

	// ���݃��x���Z�b�g
	PartsCounter *pPartsCurrLevel = GetPartsCounter("currLevel");
	if( pPartsCurrLevel ){
		pPartsCurrLevel->SetValue( currentLv+1 );
		pPartsCurrLevel->SetCountAnimFlg( false );
	}

	// �ő僌�x���Z�b�g
	PartsCounter *pPartsMaxLevel = GetPartsCounter("levelMax");
	if( pPartsMaxLevel ){
		if( kind == Common::BASE_STATE_CONTINUE ){
			pPartsMaxLevel->SetValue( 2 );
		}
		else{
			pPartsMaxLevel->SetValue( 10 );
		}
		pPartsMaxLevel->SetCountAnimFlg( false );
	}

	// ����Lv�܂ł̃|�C���g�Z�b�g
	PartsCounter *pPartsPointToNext = GetPartsCounter("nextPoint");
	if( pPartsPointToNext ){
		if( pointToNextLv == INVALID_VALUE ){
			pPartsPointToNext->SetValue( 9999 );
		}
		else{
			pPartsPointToNext->SetValue( pointToNextLv );
		}
		pPartsPointToNext->SetCountAnimFlg( false );
	}

	// �����|�C���g�Z�b�g
	PartsCounter *pPartsCurrPoint = GetPartsCounter("currPoint");
	if( pPartsCurrPoint ){
		pPartsCurrPoint->SetValue( m_loadData.m_battlePoint );
		pPartsCurrPoint->SetCountAnimFlg( false );
	}

	// ���x���A�b�v�\���ǂ���
	MenuParts *pPartsDecision = GetParts( "decisionUp" );
	if( pPartsDecision ){
		if( pointToNextLv <= m_loadData.m_battlePoint ){
			// LvUp�\
			SetAnim( "decisionUp", "possible" );
		}
		else{
			SetAnim( "decisionUp", "inpossible" );
		}
	}

	// �������X�V
	m_explanationStr = GetExplanationStr( kind );
}

/* ================================================ */
/**
 * @brief	�J�[�\���ړ��̏����܂Ƃ�
 */
/* ================================================ */
void PowerUpMenu::UpdateCursorMove()
{
	if( GetSelectedNo() == SEELCT_ITEM ){
		SetAnim( "frame", "spot" );
	}
	else{
		SetAnim( "frame", "idle" );
	}
}

void PowerUpMenu::UpdateMenu()
{
	CallPadEvent();
	
	// �������Z�b�g
	Draw2DManager::GetInstance()->PushDrawString( m_explanationStr, math::Vector2( 300.0f, 430.0f ) );
}

/* ================================================ */
/**
 * @brief	�p�b�h����֐�
 */
/* ================================================ */
void PowerUpMenu::PadEventDecide()
{
	if( GetSelectedNo() == SELECT_GAME_START ){
		// ����SE�炷
		SoundManager::GetInstance()->PlaySE("Decide");

		// �R���e�B�j���[�o���邩�ǂ����`�F�b�N
		if( m_loadData.m_playerBaseStateLv[Common::BASE_STATE_CONTINUE] == 0 ){
			// ���ڃQ�[����ʂ�
			SetNextFlowStr( "proceed" );
		}
		else{
			// �X�e�[�W�Z���N�g��
			SetNextFlowStr( "stageselect" );
		}

		// ����i�s�t���O�𗧂Ă� = PP��0
		m_isProceedEnd = true;
	}
	else{

		uint32_t needPoint = GetPointToNextLevel( m_selectStateKind, m_loadData.m_playerBaseStateLv[m_selectStateKind] );
		if( m_loadData.m_battlePoint < needPoint
			|| needPoint == INVALID_VALUE )
		{
			// �|�C���g������Ȃ� or Lv���ő�
			// �G���[��
			SoundManager::GetInstance()->PlaySE("Error");
			return;
		}

		// LvUp���ʉ�
		SoundManager::GetInstance()->PlaySE("LearningAction");

		// �K�v�|�C���g���������҂�
		m_loadData.m_battlePoint -= needPoint;
		
		// ���ڂ̃p���[�A�b�v
		m_loadData.m_playerBaseStateLv[m_selectStateKind] += 1;

		// �\���X�V
		ChangeDispState( m_selectStateKind );
	}
}

void PowerUpMenu::PadEventUp()
{
	// �J�[�\��SE�炷
	SoundManager::GetInstance()->PlaySE("Cursor");

	uint32_t selectNo = ( GetSelectedNo() + 1 ) % SELECT_MAX;
	SetSelectNum( selectNo );

	// �`��X�V
	UpdateCursorMove();
}

void PowerUpMenu::PadEventDown()
{
	// �J�[�\��SE�炷
	SoundManager::GetInstance()->PlaySE("Cursor");

	uint32_t selectNo = ( GetSelectedNo() + (SELECT_MAX - 1) ) % SELECT_MAX;
	SetSelectNum( selectNo );

	// �`��X�V
	UpdateCursorMove();
}

void PowerUpMenu::PadEventRight()
{
	if( GetSelectedNo() != SEELCT_ITEM ){
		// �J�[�\����GameStart�ɂ���̂ŉ������Ȃ�
		return;
	}

	// �J�[�\��SE�炷
	SoundManager::GetInstance()->PlaySE("Cursor");

	uint32_t selectNo = ( static_cast<uint32_t>(m_selectStateKind) + 1 ) % Common::BASE_STATE_MAX;
	m_selectStateKind = static_cast<Common::PLAYER_BASE_STATE>(selectNo);
	ChangeDispState( m_selectStateKind );
}

void PowerUpMenu::PadEventLeft()
{
	if( GetSelectedNo() != SEELCT_ITEM ){
		// �J�[�\����GameStart�ɂ���̂ŉ������Ȃ�
		return;
	}

	// �J�[�\��SE�炷
	SoundManager::GetInstance()->PlaySE("Cursor");

	uint32_t selectNo = ( static_cast<uint32_t>(m_selectStateKind) + (Common::BASE_STATE_MAX - 1) ) % Common::BASE_STATE_MAX;
	m_selectStateKind = static_cast<Common::PLAYER_BASE_STATE>(selectNo);
	ChangeDispState( m_selectStateKind );
}

void PowerUpMenu::PadEventCommand( const uint32_t &commandKind )
{
	if( commandKind == KONAMI_COMMAND_SUCCESS ){
		// �J�[�\��SE�炷
		SoundManager::GetInstance()->PlaySE("konamiCommand");
		// �����|�C���g���ő��
		m_loadData.m_battlePoint = 9998;
		// �\���X�V
		ChangeDispState( m_selectStateKind );
	}
}

/* ================================================ */
/**
 * @brief	���ڂ��Ƃ̐������擾
 */
/* ================================================ */
std::string PowerUpMenu::GetExplanationStr( const Common::PLAYER_BASE_STATE &kind )
{
	std::string retStr = "";
	switch( kind ){
	case Common::BASE_STATE_LIFE:		// ���C�t�̍ő�l�����߂�
		retStr = "�Q�[�����̃��C�t�̍ő�l�������܂��B";
		break;

	case Common::BASE_STATE_MOVE_SPEED:	// �_�b�V��(�ړ����x)����
		retStr = "L1��������R1���������ƂŃ_�b�V�����g�p���邱�Ƃ��ł���\n�悤�ɂȂ�܂��B";
		retStr += "Level���オ��ƃ_�b�V�����Ԃ��������܂��B";
		break;

	case Common::BASE_STATE_DEFFENCE:	// ��_���[�W�����߂�
		retStr = "�G����󂯂�_���[�W���y�����܂��B";
		break;

	case Common::BASE_STATE_BULLET_SPD:	// �}�V���K���̊Ԋu
		retStr = "�E�X�e�B�b�N�Ŕ��˂���e�ۂ̔��ˊ��o���Z���Ȃ�A\n��葽���̒e�����˂����悤�ɂȂ�܂��B";
		break;

	case Common::BASE_STATE_BULLET_DMG:	// �}�V���K���̃_���[�W
		retStr = "�E�X�e�B�b�N�Ŕ��˂���e�ۂ̈З͂��������܂��B";
		break;
	
	case Common::BASE_STATE_BLADE_LEVEL:	// �a���̃_���[�W
		retStr = "����L�[�ŏo����a���̈З͂��������܂��B";
		break;
	
	case Common::BASE_STATE_CONTINUE:		// �R���e�B�j���[
		retStr = "�Q�[�����J�n����Ƃ��ɈȑO�N���A����Stage����n�߂�\n���Ƃ��ł���悤�ɂȂ�܂��B";
		break;
	}
	return retStr;
}


/* ================================================ */
/**
 * @brief	���̃��x���܂ł̃|�C���g���擾
 */
/* ================================================ */
uint32_t PowerUpMenu::GetPointToNextLevel( const Common::PLAYER_BASE_STATE &kind, uint32_t currLevel )
{
	
	static const uint32_t levelPointTableLow[] = {
		10,20,30,40,50,60,70,80,100
	};
	static const uint32_t levelPointTableMiddle[] = {
		10,20,30,40,50,60,80,100,120
	};
	static const uint32_t levelPointTableHigh[] = {
		10,20,30,50,70,90,110,140,170,200
	};

	uint32_t retVal = 0;

	switch( kind ){
	case Common::BASE_STATE_LIFE:				// ���C�t�̍ő�l�����߂�
		retVal = levelPointTableLow[currLevel];
		break;

	case Common::BASE_STATE_MOVE_SPEED:			// �_�b�V��(�ړ����x)����
		retVal = levelPointTableLow[currLevel];
		break;

	case Common::BASE_STATE_DEFFENCE:			// ��_���[�W�����߂�
		retVal = levelPointTableMiddle[currLevel];
		break;

	case Common::BASE_STATE_BULLET_SPD:			// �}�V���K���̊Ԋu
		retVal = levelPointTableMiddle[currLevel];
		break;

	case Common::BASE_STATE_BULLET_DMG:			// �}�V���K���̃_���[�W
		retVal = levelPointTableHigh[currLevel];
		break;
	
	case Common::BASE_STATE_BLADE_LEVEL:		// �a���̃_���[�W
		retVal = levelPointTableHigh[currLevel];
		break;

	case Common::BASE_STATE_CONTINUE:		// �R���e�B�j���[
		retVal = ( currLevel == 0 ) ? 300 : 9999 ;
		break;
	}

	if( currLevel >= Common::STATUS_LEVEL_MAX ){
		// ���ɍő�
		return INVALID_VALUE;
	}

	return retVal;
}