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

void GameRecorder::InitRecord()
{
	m_userScore = 0;
	for( uint32_t i = 0; i < NUMBEROF(m_scoreDetail) ; ++i ){
		m_scoreDetail[i] = 0;
	}
}

void GameRecorder::ScoreEvent( const SCORE_KIND &kind )
{
	uint32_t addValue = 0;
	switch( kind ){

	default:
	case POINT_MAX:
		DEBUG_ASSERT( 0, "�X�R�A���Z�̎�ނ���������" );
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
	m_userScore += addValue;

	++m_scoreDetail[static_cast<uint32_t>(kind)];
}

const int32_t GameRecorder::GetScore() const
{
	return m_userScore;
}

// �Q�[���i�s��
void GameRecorder::SetGameStateOfProgress( STATE_OF_PROGRESS nextState )
{
	m_gameState = nextState;
}
const GameRecorder::STATE_OF_PROGRESS &GameRecorder::GetGameStateOfProgress() const
{
	return m_gameState;
}