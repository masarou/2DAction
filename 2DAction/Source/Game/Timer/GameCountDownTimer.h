/* ====================================================================== */
/**
 * @brief  制限時間付きステージで出す残り時間クラス
 *
 * @note
 *		
 */
/* ====================================================================== */

#ifndef __GAME_COUNTDOWN_TIMER__
#define __GAME_COUNTDOWN_TIMER__

#include "System/Task/SystemTaskUnit.h"
#include "System/Draw2D/SystemDraw2DDefine.h"

class NumberCounter;

class GameCountDownTimer: public TaskUnit
{
	
public:

	enum DISP_TYPE{
		DISP_FRAME,
		DISP_SEC,

		DISP_MAX,
	};

	static GameCountDownTimer *CreateByFrame( uint32_t startFrameNum, bool isDispCountDown = true );
	static GameCountDownTimer *CreateBySec( uint32_t startSecNum, bool isDispCountDown = true );

	void	SetPosition( const math::Vector2 &pos );
	bool	IsCountDownEnd(){ return m_currentNum == 0 ? true : false ; }

protected:

	virtual bool Init() override;
	virtual void Update() override;
	virtual bool DieMain() override;

private:

	// DISP_TYPEを見て表示すべき数字を返す
	const uint32_t ConvDispNum() const;

	GameCountDownTimer( uint32_t startNum, DISP_TYPE type, bool isDispCountDown );
	~GameCountDownTimer(void);
	
	NumberCounter		*m_pNumCounter;			// スコア表示
	math::Vector2		m_dispPos;

	bool				m_isDisp;				// カウントダウンを表示するかどうか
	DISP_TYPE			m_type;					// フレームか秒数か
	uint32_t			m_startNum;				// 開始数
	uint32_t			m_currentNum;			// 現在値

};

#endif