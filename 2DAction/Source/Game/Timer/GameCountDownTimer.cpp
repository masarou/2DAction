/* ====================================================================== */
/**
 * @brief  制限時間付きステージで出す残り時間クラス
 *
 * @note
 *		
 */
/* ====================================================================== */

#include "GameCountDownTimer.h"
#include "Common/CmnNumberCounter.h"

GameCountDownTimer *GameCountDownTimer::CreateByFrame( uint32_t startFrameNum, bool isDispCountDown )
{
	return NEW GameCountDownTimer( startFrameNum, GameCountDownTimer::DISP_FRAME, isDispCountDown );
}

GameCountDownTimer *GameCountDownTimer::CreateBySec( uint32_t startSecNum, bool isDispCountDown )
{
	return NEW GameCountDownTimer( startSecNum, GameCountDownTimer::DISP_SEC, isDispCountDown );
}

GameCountDownTimer::GameCountDownTimer( uint32_t startNum, DISP_TYPE type, bool isDispCountDown )
: m_pNumCounter( NULL )
, m_isDisp( isDispCountDown )
, m_type( type )
, m_startNum( startNum )
, m_currentNum( startNum )
{
	m_dispPos = math::Vector2();
}


GameCountDownTimer::~GameCountDownTimer(void)
{
}

void GameCountDownTimer::SetPosition( const math::Vector2 &pos )
{
	// Init前の可能性があるので保持しておく
	m_dispPos = pos;
	if( m_pNumCounter ){
		m_pNumCounter->SetDrawPos( m_dispPos );
	}
}

bool GameCountDownTimer::Init()
{
	if( m_type == DISP_SEC ){
		// 秒数指定なら60倍にしてFrameとして内部的に扱う
		m_currentNum *= 60;
	}

	if( m_isDisp ){
		m_pNumCounter = NumberCounter::Create("Number.json");
		m_pNumCounter->SetDrawPos( m_dispPos );
		m_pNumCounter->SetValue( ConvDispNum() );
		m_pNumCounter->CountAnimEnd();
	}

	return true;
}

void GameCountDownTimer::Update()
{
	if( m_currentNum > 0 ){
		--m_currentNum;
	}
	if( m_pNumCounter ){
		m_pNumCounter->SetValue( ConvDispNum() );
	}
}

bool GameCountDownTimer::DieMain()
{
	return true;
}


const uint32_t GameCountDownTimer::ConvDispNum() const
{
	uint32_t retVal = m_currentNum;
	if( m_type == DISP_SEC ){
		if( retVal != 0 ){
			retVal += 59;
			retVal /= 60;
		}
	}
	return retVal;
}
