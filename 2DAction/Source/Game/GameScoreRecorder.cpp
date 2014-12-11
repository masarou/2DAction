/* ====================================================================== */
/**
 * @brief  スコア記録クラス
 *
 * @note
 *		
 */
/* ====================================================================== */

#include "GameScoreRecorder.h"
#include "System/Draw2D/SystemDraw2DResource.h"
#include "Flow/FlowManager.h"

ScoreRecorder *ScoreRecorder::s_pInstance = NULL;

ScoreRecorder *ScoreRecorder::Create()
{
	if( !s_pInstance ){
		s_pInstance = NEW ScoreRecorder();
	}
	else{
		DEBUG_ASSERT( 0, "インスタンスが既にあります : ScoreRecorder" );
	}
	return s_pInstance;
}

ScoreRecorder *ScoreRecorder::GetInstance()
{
	if( !s_pInstance ){
		s_pInstance = Create();
	}
	return s_pInstance;
}

ScoreRecorder::ScoreRecorder()
: TaskUnit("GameScoreRecorder")
, m_pNumCounter( NULL )
{
	m_pNumCounter = NumberCounter::Create("number.json");
}

ScoreRecorder::~ScoreRecorder()
{
	s_pInstance = NULL;
}

void ScoreRecorder::ScoreReset()
{
	if( !m_pNumCounter ){
		DEBUG_ASSERT( 0, "m_pNumCounter is NULL ");
		return;
	}
	m_pNumCounter->Reset();
}

void ScoreRecorder::ScoreEvent( const SCORE_KIND &kind )
{
	if( !m_pNumCounter ){
		DEBUG_ASSERT( 0, "m_pNumCounter is NULL ");
		return;
	}

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
	m_pNumCounter->AddValue( addValue );

	++m_scoreDetail[static_cast<uint32_t>(kind)];
}

const int32_t ScoreRecorder::GetScore() const
{
	int32_t score = 0;
	if( m_pNumCounter ){
		score = m_pNumCounter->GetValue();
	}
	return score;
}