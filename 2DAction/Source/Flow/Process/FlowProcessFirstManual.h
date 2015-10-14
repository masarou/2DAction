/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		����������o�N���X
 */
/* ====================================================================== */
#ifndef __FLOW_PROCESS_FIRST_MANUAL__
#define __FLOW_PROCESS_FIRST_MANUAL__

#include "FlowProcessBase.h"

class FirstManual : public ProcessBase
{
public:

	enum MANUAL_KIND{
		KIND_POWERUP1,
		KIND_POWERUP2,
		KIND_POWERUP3,
		KIND_POWERUP4,
		KIND_POWERUP5,
		KIND_POWERUP6,
		
		KIND_GAMEPLAY01,
		KIND_GAMEPLAY02,

		KIND_GAMEADDRESS,

		KIND_MAX,
	};

	static FirstManual *Create( const MANUAL_KIND &kind );

	~FirstManual(void);

protected:

	virtual bool Init() override;
	virtual void Update() override;
	virtual bool CanDie() override;

	// �p�b�h����
	virtual void PadEventDecide() override;
	
	// ���t�B���^�[���K�v���ǂ���
	virtual bool IsNeedPauseFilter() const;

private:

	enum STEP_ANIM{
		STEP_IN,
		STEP_WAIT,
		STEP_OUT,
		STEP_END,

		STEP_MAX,
	};

	const std::string GetFilePath() const;
	void ChangeAnimeStepNext();

	FirstManual( const MANUAL_KIND &kind );

	Texture2D			m_controllTex;	// ����摜�f�[�^

	MANUAL_KIND			m_kind;
	STEP_ANIM			m_step;
};

#endif