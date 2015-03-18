/* ====================================================================== */
/**
 * @brief  �������ԕt���X�e�[�W�ŏo���c�莞�ԃN���X
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

	// DISP_TYPE�����ĕ\�����ׂ�������Ԃ�
	const uint32_t ConvDispNum() const;

	GameCountDownTimer( uint32_t startNum, DISP_TYPE type, bool isDispCountDown );
	~GameCountDownTimer(void);
	
	NumberCounter		*m_pNumCounter;			// �X�R�A�\��
	math::Vector2		m_dispPos;

	bool				m_isDisp;				// �J�E���g�_�E����\�����邩�ǂ���
	DISP_TYPE			m_type;					// �t���[�����b����
	uint32_t			m_startNum;				// �J�n��
	uint32_t			m_currentNum;			// ���ݒl

};

#endif