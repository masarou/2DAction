/* ====================================================================== */
/**
 * @brief  �X�R�A�L�^�N���X
 *
 * @note
 *		
 */
/* ====================================================================== */

#include "GameRecorder.h"
#include "System/Draw2D/SystemDraw2DResource.h"
#include "Flow/FlowManager.h"

GameRecorder *GameRecorder::s_pInstance = NULL;

GameRecorder *GameRecorder::Create()
{
	if( !s_pInstance ){
		s_pInstance = NEW GameRecorder();
	}
	else{
		DEBUG_ASSERT( 0, "�C���X�^���X�����ɂ���܂� : GameRecorder" );
	}
	return s_pInstance;
}

GameRecorder *GameRecorder::GetInstance()
{
	if( !s_pInstance ){
		s_pInstance = Create();
	}
	return s_pInstance;
}

GameRecorder::GameRecorder()
: TaskUnit("GameScoreRecorder")
, m_gameState( STATE_MAX )
{
	InitRecord();
}

GameRecorder::~GameRecorder()
{
	s_pInstance = NULL;
}

/* ================================================ */
/**
 * @brief	������
 */
/* ================================================ */
void GameRecorder::InitRecord()
{
	for( uint32_t i = 0; i < NUMBEROF(m_clearStageInfo) ; ++i ){
		m_clearStageInfo[i].Init();
	}
	for( uint32_t i = 0; i < NUMBEROF(m_getItem) ; ++i ){
		m_getItem[i] = 0;
	}
	m_isContinuePlay = false;
	m_isAllClear = false;
	m_hitComboNum = 0;
	m_hitFailTime_ms = 0;
}

/* ================================================ */
/**
 * @brief	�X�e�[�W�I�����̃��[�U�[���C�t�̊����̎擾�ƃZ�b�g
 */
/* ================================================ */
const void GameRecorder::SetUserLifeRatio( const float &lifeRatio, const STATE_OF_PROGRESS &stage )
{
	if( stage == STATE_MAX ){
		m_clearStageInfo[static_cast<uint32_t>(m_gameState)].m_userLifeRatio = lifeRatio;
	}
	else{
		m_clearStageInfo[static_cast<uint32_t>(stage)].m_userLifeRatio = lifeRatio;
	}
}
const float GameRecorder::GetUserLifeRatio( const STATE_OF_PROGRESS &stage ) const
{
	float retVal = 0.0f;
	if( stage == STATE_MAX ){
		retVal = m_clearStageInfo[static_cast<uint32_t>(m_gameState)].m_userLifeRatio;
	}
	else{
		retVal = m_clearStageInfo[static_cast<uint32_t>(stage)].m_userLifeRatio;
	}
	return retVal;
}

/* ================================================ */
/**
 * @brief	���[�U�[���w��X�e�[�W���N���A�������ǂ���
 */
/* ================================================ */
const bool GameRecorder::IsUserClear( const STATE_OF_PROGRESS &stage ) const
{
	float ratio = 0.0f;
	if( stage == STATE_MAX ){
		ratio = m_clearStageInfo[static_cast<uint32_t>(m_gameState)].m_userLifeRatio;
	}
	else{
		ratio = m_clearStageInfo[static_cast<uint32_t>(stage)].m_userLifeRatio;
	}
	return ( ratio == 0.0f ) ? false : true ;
}

/* ================================================ */
/**
 * @brief	�X�R�A�ϓ��C�x���g�����������ɌĂ΂��
 */
/* ================================================ */
void GameRecorder::ScoreEvent( const SCORE_KIND &kind, const uint32_t &tmpInfo )
{
	uint32_t addValue = 0;
	switch( kind ){
	default:
	case POINT_MAX:
		DEBUG_ASSERT( 0, "�X�R�A���Z�̎�ނ���������" );
		return;
	case ENEMY_SLIME_DEATH:
		addValue = 100 + ( 100 * tmpInfo );
		break;
	case ENEMY_SLIME_ANOTHER_DEATH:
		addValue = 100 + ( 50 * tmpInfo );
		break;
	case ENEMY_AHRIMAN_DEATH:
		addValue = 300 + ( 50 * tmpInfo );
		break;
	case ENEMY_COW_DEATH:
		addValue = 700 + ( 100 * tmpInfo );
		break;
	case ENEMY_SLIME_KING_DEATH:
		addValue = 10000 + ( 500 * tmpInfo );
		break;
	case ENEMY_WIZARD_DEATH:
		addValue = 15000 + ( 800 * tmpInfo );
		break;
	case ENEMY_DRAGON_DEATH:
		addValue = 20000 + ( 800 * tmpInfo );
		break;
	case ENEMY_BOSS_DEATH:
		addValue = 30000 + ( 1000 * tmpInfo );
		break;
	case ENEMY_LAST_BOSS_DEATH:
		addValue = 100000;
		break;
	case ENEMY_LAST_BOSS_RIGHT_DEATH:
		addValue = 4000;
		break;
	case ENEMY_LAST_BOSS_LEFT_DEATH:
		addValue = 4000;
		break;
	}

	m_clearStageInfo[static_cast<uint32_t>(m_gameState)].m_userScore += addValue;

	++m_clearStageInfo[static_cast<uint32_t>(m_gameState)].m_scoreDetail[static_cast<uint32_t>(kind)];
}

/* ================================================ */
/**
 * @brief	�X�e�[�W�I�����̃{�[�i�X�X�R�A�̃Z�b�g
 */
/* ================================================ */
void GameRecorder::AddScoreBonus( uint32_t bonusScore, const STATE_OF_PROGRESS &stage )
{
	if( stage == STATE_MAX ){
		m_clearStageInfo[static_cast<uint32_t>(m_gameState)].m_bonusScore += bonusScore;
	}
	else{
		m_clearStageInfo[static_cast<uint32_t>(stage)].m_bonusScore += bonusScore;
	}
}

/* ================================================ */
/**
 * @brief	�e�X�e�[�W�̃X�R�A�擾
 */
/* ================================================ */
const int32_t GameRecorder::GetScore( const STATE_OF_PROGRESS &stage ) const
{
	uint32_t score = 0;
	if( stage == STATE_MAX ){
		score = m_clearStageInfo[static_cast<uint32_t>(m_gameState)].m_userScore;
	}
	else{
		score = m_clearStageInfo[static_cast<uint32_t>(stage)].m_userScore;
	}
	return score;
}

/* ================================================ */
/**
 * @brief	�e�X�e�[�W�̃{�[�i�X�X�R�A�擾
 */
/* ================================================ */
const int32_t GameRecorder::GetScoreBonus( const STATE_OF_PROGRESS &stage ) const
{
	uint32_t score = 0;
	if( stage == STATE_MAX ){
		score = m_clearStageInfo[static_cast<uint32_t>(m_gameState)].m_bonusScore;
	}
	else{
		score = m_clearStageInfo[static_cast<uint32_t>(stage)].m_bonusScore;
	}
	return score;
}

/* ================================================ */
/**
 * @brief	�e�X�e�[�W�̃{�[�i�X���������X�R�A�擾
 */
/* ================================================ */
const int32_t GameRecorder::GetStageScore( const STATE_OF_PROGRESS &stage ) const
{
	uint32_t score = 0;
	if( stage == STATE_MAX ){
		score += m_clearStageInfo[static_cast<uint32_t>(m_gameState)].m_userScore;
		score += m_clearStageInfo[static_cast<uint32_t>(m_gameState)].m_bonusScore;
	}
	else{
		score += m_clearStageInfo[static_cast<uint32_t>(stage)].m_userScore;
		score += m_clearStageInfo[static_cast<uint32_t>(stage)].m_bonusScore;
	}
	return score;
}

/* ================================================ */
/**
 * @brief	�S�X�e�[�W�̃X�R�A�Ƀ{�[�i�X���������X�R�A�擾
 */
/* ================================================ */
const int32_t GameRecorder::GetTotalScore() const
{
	uint32_t score = 0;
	for( uint32_t i = 0; i < STATE_MAX ; ++i ){
		score += m_clearStageInfo[i].m_userScore + m_clearStageInfo[i].m_bonusScore;
	}
	return score;
}

/* ================================================ */
/**
 * @brief	�A�C�e���擾����ǉ��A�J�E���g
 */
/* ================================================ */
const void GameRecorder::AddItem( Common::ITEM_KIND kind )
{
	uint32_t index = static_cast<uint32_t>(kind);
	++m_getItem[index];
}

const uint32_t GameRecorder::GetItemCount( Common::ITEM_KIND kind ) const
{
	uint32_t index = static_cast<uint32_t>(kind);
	return m_getItem[index];
}

/* ================================================ */
/**
 * @brief	�Q�[���i�s�󋵂̍X�V�Ǝ擾
 */
/* ================================================ */
void GameRecorder::SetGameStateOfProgress( STATE_OF_PROGRESS nextState )
{
	m_gameState = nextState;
}
const GameRecorder::STATE_OF_PROGRESS &GameRecorder::GetGameStateOfProgress() const
{
	return m_gameState;
}

/* ================================================ */
/**
 * @brief	�R���{Hit����Inc
 */
/* ================================================ */
void GameRecorder::IncHitCounter()
{
	++m_hitComboNum;
	m_hitFailTime_ms = COMBO_COUNT_MAX;
}

/* ================================================ */
/**
 * @brief	���݂̘A��Hit���擾
 */
/* ================================================ */
uint32_t GameRecorder::GetCurrentHitCounter()
{
	return m_hitComboNum;
}

/* ================================================ */
/**
 * @brief	�e�X�e�[�W�ł̍ő�Hit���擾
 */
/* ================================================ */
uint32_t GameRecorder::GetMaxComboNumOfStage( const STATE_OF_PROGRESS &stage ) const
{
	uint32_t comboNum = 0;
	if( stage == STATE_MAX ){
		comboNum = m_clearStageInfo[static_cast<uint32_t>(m_gameState)].m_hitComboMaxOfStage;
	}
	else{
		comboNum = m_clearStageInfo[static_cast<uint32_t>(stage)].m_hitComboMaxOfStage;
	}
	return comboNum;
}

/* ================================================ */
/**
 * @brief	BattlePoint(PP)�̎擾�ƃZ�b�g
 */
/* ================================================ */
void GameRecorder::SetClearBattlePoint( const uint32_t &score, const uint32_t &point, const STATE_OF_PROGRESS &stage )
{
	BATTLE_POINT_INFO info = { score, point };
	if( stage == STATE_MAX ){
		m_clearStageInfo[static_cast<uint32_t>(m_gameState)].m_clearPoint.push_back( info );
	}
	else{
		m_clearStageInfo[static_cast<uint32_t>(stage)].m_clearPoint.push_back( info );
	}
}
uint32_t GameRecorder::GetClearBattlePoint( const uint32_t &point, const STATE_OF_PROGRESS &stage ) const
{
	// �w�肳�ꂽ�|�C���g����A�擾�ł���o�g���|�C���g�����߂�
	STATE_OF_PROGRESS flowState = STATE_MAX;
	if( stage == STATE_MAX ){
		flowState = m_gameState;
	}
	else{
		flowState = stage;
	}

	uint32_t diff = 0;
	uint32_t retVal = 0;
	for( uint32_t i = 0; i < m_clearStageInfo[static_cast<uint32_t>(flowState)].m_clearPoint.size();++i ){
		if( m_clearStageInfo[static_cast<uint32_t>(flowState)].m_clearPoint.at(i).m_scoreUnder < point ){
			uint32_t tmpDiff = point - m_clearStageInfo[static_cast<uint32_t>(flowState)].m_clearPoint.at(i).m_scoreUnder;
			if( diff == 0 || tmpDiff < diff ){
				retVal = m_clearStageInfo[static_cast<uint32_t>(flowState)].m_clearPoint.at(i).m_battlePoint;
				diff = tmpDiff;
			}
		}
	}
	return retVal;
}

/* ================================================ */
/**
 * @brief	BattlePoint(PP)���Z�b�g
 */
/* ================================================ */
uint32_t GameRecorder::GetClearBattlePointAll() const
{
	uint32_t retVal = 0;
	for( uint32_t i = 0 ; i < STATE_MAX ; ++i ){
		STATE_OF_PROGRESS stage = static_cast< STATE_OF_PROGRESS >(i);
		retVal += GetClearBattlePoint( GetStageScore( stage ), stage );
	}
	return retVal;
}

/* ================================================ */
/**
 * @brief	�R���{�p���L�����Ԏ擾
 */
/* ================================================ */
uint32_t GameRecorder::GetLeftTimeOfCombo() const
{
	return m_hitFailTime_ms;
}

/* ================================================ */
/**
 * @brief	���t���[���X�V
 */
/* ================================================ */
void GameRecorder::Update()
{
	// Hit�J�E���^����
	if( m_hitFailTime_ms > 0 ){
		uint32_t subtraction = 0;
		if( m_hitComboNum > 70 ){
			subtraction = COMBO_COUNT_MAX / 30;
		}
		else if( m_hitComboNum > 60 ){
			subtraction = COMBO_COUNT_MAX / 40;
		}
		else if( m_hitComboNum > 50 ){
			subtraction = COMBO_COUNT_MAX / 50;
		}
		else if( m_hitComboNum > 40 ){
			subtraction = COMBO_COUNT_MAX / 60;
		}
		else if( m_hitComboNum > 30 ){
			subtraction = COMBO_COUNT_MAX / 70;
		}
		else if( m_hitComboNum > 20 ){
			subtraction = COMBO_COUNT_MAX / 80;
		}
		else if( m_hitComboNum > 10 ){
			subtraction = COMBO_COUNT_MAX / 90;
		}
		else{
			subtraction = 100;
		}
		if( subtraction < m_hitFailTime_ms ){
			m_hitFailTime_ms -= subtraction;
		}
		else{
			m_hitFailTime_ms = 0;
		}
	}
	else{
		m_hitFailTime_ms	= 0;
		m_hitComboNum		= 0;
	}

	if( m_clearStageInfo[static_cast<uint32_t>(m_gameState)].m_hitComboMaxOfStage < m_hitComboNum ){
		// �ő�R���{���X�V!!!
		m_clearStageInfo[static_cast<uint32_t>(m_gameState)].m_hitComboMaxOfStage = m_hitComboNum;
	}
}