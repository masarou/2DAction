/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		�X�e�[�W�����N���X
 */
/* ====================================================================== */
#ifndef __FLOW_PROCESS_INIT_LAST_STAGE__
#define __FLOW_PROCESS_INIT_LAST_STAGE__

#include "FlowProcessBase.h"

class StageStart : public ProcessBase
{
public:

	static StageStart *Create();

	~StageStart(void);

protected:

	virtual bool Init() override;
	virtual void Update() override;
	virtual bool CanDie() override;
	virtual void PadEventDecide() override;
	virtual bool IsNeedPauseFilter() const override{ return false; }

private:

	void FadeUpdate( TEX_DRAW_INFO &info );
	std::string GetStageAnimStr();
	std::string GetClearAnimStr();

	StageStart(void);

	uint32_t		m_counter;			// ���o���J�n���Ă���̃J�E���^
	int32_t			m_texSizeW;			// �e�N�X�`���T�C�Y��ʂ���o�����ǂ����̔���Ŏg�p
	bool			m_IsFadeOut;		// �t�F�[�h�A�E�g���J�n���Ă��悢���ǂ���
	Texture2D		m_stagePhaseTex;	// �X�e�[�W����������\���摜
	Texture2D		m_stageClearTex;	// �N���A������\���摜
};

#endif