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
	for( uint32_t i = 0; i < STATE_MAX ; ++i ){
		m_userScore[i] = 0;
	}
	for( uint32_t i = 0; i < NUMBEROF(m_scoreDetail) ; ++i ){
		m_scoreDetail[i] = 0;
	}
	for( uint32_t i = 0; i < NUMBEROF(m_getItem) ; ++i ){
		m_getItem[i] = 0;
	}
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
	m_userScore[static_cast<uint32_t>(m_gameState)] += addValue;

	++m_scoreDetail[static_cast<uint32_t>(kind)];
}

const int32_t GameRecorder::GetScore( const STATE_OF_PROGRESS &stage ) const
{
	uint32_t score = 0;
	if( stage == STATE_MAX ){
		score = m_userScore[static_cast<uint32_t>(m_gameState)];
	}
	else{
		score = m_userScore[static_cast<uint32_t>(stage)];
	}
	return score;
}

const int32_t GameRecorder::GetTotalScore() const
{
	uint32_t score = 0;
	for( uint32_t i = 0; i < STATE_MAX ; ++i ){
		score += m_userScore[i];
	}
	return score;
}

const void GameRecorder::AddItem( ItemObject::ITEM_KIND kind )
{
	uint32_t index = static_cast<uint32_t>(kind);
	++m_getItem[index];
}

const uint32_t GameRecorder::GetItemCount( ItemObject::ITEM_KIND kind )
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