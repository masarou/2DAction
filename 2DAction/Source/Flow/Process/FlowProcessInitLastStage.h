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

class InitLastStage : public ProcessBase
{
public:

	static InitLastStage *Create();

	~InitLastStage(void);

protected:

	virtual bool Init() override;
	virtual void Update() override;
	virtual bool CanDie() override;

	// �p�b�h����
	void PadEventDecide() override;

	InitLastStage(void);

	Texture2D			m_controllTex;	// ����摜�f�[�^
};

#endif