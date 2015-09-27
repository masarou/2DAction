/* ====================================================================== */
/**
 * @brief  Pause����ʂ𕢂�����
 *
 * @note
 *		
 */
/* ====================================================================== */
#ifndef __PAUSE_FILTER__
#define __PAUSE_FILTER__

#include "Game/Game2DBase.h"
#include "System/Task/SystemTaskUnit.h"

class PauseFilter : public TaskUnit
{

public:

	static PauseFilter *Create();
	static PauseFilter *GetInstance();
	PauseFilter(void);
	~PauseFilter(void);

	// ���擾
	void	Show();
	void	Hide();

protected:

	virtual bool DieMain() override;
	virtual void DrawUpdate() override;

private:

	static PauseFilter	*s_pInstance;

	Texture2D			m_drawTexture;	// �\���摜�f�[�^
	Texture2D			m_drawPauseStr;	// Pause�摜�f�[�^

};
#endif // FLOW_FADE_MANAGER