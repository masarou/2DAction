/* ====================================================================== */
/**
 * @brief  スコア記録クラス
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
		DEBUG_ASSERT( 0, "インスタンスが既にあります : GameRecorder" );
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

void GameRecorder::InitRecord()
{
	for( uint32_t i = 0; i < NUMBEROF(m_clearStageInfo) ; ++i ){
		m_clearStageInfo[i].Init();
	}
	for( uint32_t i = 0; i < NUMBEROF(m_getItem) ; ++i ){
		m_getItem[i] = 0;
	}
	m_hitComboNum = 0;
	m_hitFailTime_ms = 0;
}

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

void GameRecorder::ScoreEvent( const SCORE_KIND &kind )
{
	uint32_t addValue = 0;
	switch( kind ){

	default:
	case POINT_MAX:
		DEBUG_ASSERT( 0, "スコア加算の種類がおかしい" );
		return;
	case ENEMY_AAA_DEATH:
		addValue = 100;
		break;
	case ENEMY_BBB_DEATH:
		addValue = 100;
		break;
	case ENEMY_CCC_DEATH:
		addValue = 100;
		break;
	}
	m_clearStageInfo[static_cast<uint32_t>(m_gameState)].m_userScore += addValue;

	++m_clearStageInfo[static_cast<uint32_t>(m_gameState)].m_scoreDetail[static_cast<uint32_t>(kind)];
}

void GameRecorder::AddScoreBonus( uint32_t bonusScore, const STATE_OF_PROGRESS &stage )
{
	if( stage == STATE_MAX ){
		m_clearStageInfo[static_cast<uint32_t>(m_gameState)].m_bonusScore += bonusScore;
	}
	else{
		m_clearStageInfo[static_cast<uint32_t>(stage)].m_bonusScore += bonusScore;
	}
}

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

const int32_t GameRecorder::GetTotalScore() const
{
	uint32_t score = 0;
	for( uint32_t i = 0; i < STATE_MAX ; ++i ){
		score += m_clearStageInfo[i].m_userScore + m_clearStageInfo[i].m_bonusScore;
	}
	return score;
}

const void GameRecorder::AddItem( ItemObject::ITEM_KIND kind )
{
	uint32_t index = static_cast<uint32_t>(kind);
	++m_getItem[index];
}

const uint32_t GameRecorder::GetItemCount( ItemObject::ITEM_KIND kind ) const
{
	uint32_t index = static_cast<uint32_t>(kind);
	return m_getItem[index];
}

// ゲーム進行状況
void GameRecorder::SetGameStateOfProgress( STATE_OF_PROGRESS nextState )
{
	m_gameState = nextState;
}
const GameRecorder::STATE_OF_PROGRESS &GameRecorder::GetGameStateOfProgress() const
{
	return m_gameState;
}

// Hit数プラス
void GameRecorder::IncHitCounter()
{
	++m_hitComboNum;
	m_hitFailTime_ms = COMBO_COUNT_MAX;
}

// 現在の連続Hit数取得
uint32_t GameRecorder::GetCurrentHitCounter()
{
	return m_hitComboNum;
}

// 連続Hitが有効になるまでの残時間
uint32_t GameRecorder::GetRestHitTime()
{
	return m_hitFailTime_ms;
}


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

// コンボ継続有効時間取得
uint32_t GameRecorder::GetLeftTimeOfCombo() const
{
	return m_hitFailTime_ms;
}

void GameRecorder::Update()
{
	// Hitカウンタ処理
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
		// 最大コンボ数更新!!!
		m_clearStageInfo[static_cast<uint32_t>(m_gameState)].m_hitComboMaxOfStage = m_hitComboNum;
	}
}