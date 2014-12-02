/* ====================================================================== */
/**
 * @brief  ��ʂ𕢂��t�F�[�h�̊Ǘ��N���X
 *
 * @note
 *		
 */
/* ====================================================================== */
#ifndef FLOW_FADE_MANAGER
#define FLOW_FADE_MANAGER

#include "Game/Game2DBase.h"
#include "System/Task/SystemTaskUnit.h"

class FadeManager : public TaskUnit
{

public:

	enum STATE_FADE{
		STATE_IDLE,			// �������Ă��Ȃ�(��\��)
		STATE_FADEIN,		// �t�F�[�h�C����
		STATE_OVERWINDOW,	// ��ʂɕ\����
		STATE_FADEOUT,		// �t�F�[�h�A�E�g��

		STATE_MAX,
	};

	static FadeManager *Create();
	static FadeManager *GetInstance();
	FadeManager(void);
	~FadeManager(void);

	// ���擾
	void	StartFadeIn();
	void	StartFadeOut();
	const STATE_FADE &GetCurrentState(){ return m_currState; }

protected:

	virtual bool DieMain() override;
	virtual void DrawUpdate() override;

private:

	static FadeManager		*s_pInstance;

	Game2DBase			*m_fadePlate2D;	// ���C�t�`��N���X
	TEX_DRAW_INFO		m_PlateInfo;		// ���C�t�`����

	STATE_FADE			m_currState;		// ���݂̏��

};
#endif // FLOW_FADE_MANAGER