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

/* ================================================ */
/**
 * @brief	初期化
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
 * @brief	ステージ終了時のユーザーライフの割合の取得とセット
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
 * @brief	ユーザーが指定ステージをクリアしたかどうか
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
 * @brief	スコア変動イベントがあった時に呼ばれる
 */
/* ================================================ */
void GameRecorder::ScoreEvent( const SCORE_KIND &kind, const uint32_t &tmpInfo )
{
	uint32_t addValue = 0;
	switch( kind ){
	default:
	case POINT_MAX:
		DEBUG_ASSERT( 0, "スコア加算の種類がおかしい" );
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
 * @brief	ステージ終了時のボーナススコアのセット
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
 * @brief	各ステージのスコア取得
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
 * @brief	各ステージのボーナススコア取得
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
 * @brief	各ステージのボーナスを加えたスコア取得
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
 * @brief	全ステージのスコアにボーナスを加えたスコア取得
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
 * @brief	アイテム取得数を追加、カウント
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
 * @brief	ゲーム進行状況の更新と取得
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
 * @brief	コンボHit数のInc
 */
/* ================================================ */
void GameRecorder::IncHitCounter()
{
	++m_hitComboNum;
	m_hitFailTime_ms = COMBO_COUNT_MAX;
}

/* ================================================ */
/**
 * @brief	現在の連続Hit数取得
 */
/* ================================================ */
uint32_t GameRecorder::GetCurrentHitCounter()
{
	return m_hitComboNum;
}

/* ================================================ */
/**
 * @brief	各ステージでの最大Hit数取得
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
 * @brief	BattlePoint(PP)の取得とセット
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
	// 指定されたポイントから、取得できるバトルポイントを求める
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
 * @brief	BattlePoint(PP)リセット
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
 * @brief	コンボ継続有効時間取得
 */
/* ================================================ */
uint32_t GameRecorder::GetLeftTimeOfCombo() const
{
	return m_hitFailTime_ms;
}

/* ================================================ */
/**
 * @brief	毎フレーム更新
 */
/* ================================================ */
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