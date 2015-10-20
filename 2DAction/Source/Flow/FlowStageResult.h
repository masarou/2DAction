/* ====================================================================== */
/**
 * @brief  
 *		�Q�[���X�e�[�W���ʉ�ʃN���X
 * @note
 *		
 */
/* ====================================================================== */
#ifndef __FLOW_STAGE_RESULT__
#define __FLOW_STAGE_RESULT__

#include "FlowMenuBase.h"
#include "Game/Game2DBase.h"
#include "System/Task/SystemTaskUnit.h"
#include "System/Menu/SystemMenuWindow.h"

class FlowStageResult : public FlowMenuBase
{
public:

	static FlowBase *Create( const std::string &fileName );

private:

	virtual bool Init() override;
	virtual void UpdateFlowAfterChildTask();
	
	FlowStageResult( const std::string &fileName );
	~FlowStageResult(void);

};


/* ====================================================================== */
/**
 * @brief  
 *			�X�e�[�W���ʂ̒��g�J�E���^�N���X
 * @note
 *		
 */
/* ====================================================================== */
class ResultStageMenu : public MenuWindow
{
public:

	static ResultStageMenu *CreateResultStageMenu( const std::string &readMenuJson );

	bool ProceedNext(){ return m_dispState == DISP_ALL ? true : false ;}		// �S�ĕ\�����I���Ď��ɐi��ł������ǂ���

protected:
	
	virtual bool InitMenu() override;
	virtual void UpdateMenu() override;
	virtual void PadEventDecide() override;

private:

	enum DISP_STATE{
		DISP_RESULT,
		DISP_BONUS,
		DISP_TOTAL,
		DISP_BATTLEPOINT,
		DISP_ALL,

		DISP_MAX,
	};

	ResultStageMenu( const std::string &readMenuJson );
	virtual ~ResultStageMenu(void);
	uint32_t GetStageClearBonus() const;

	// �J�E���g���ڂ�����(true�őS���\����������)
	bool	ChangeDispStateNext();

	DISP_STATE			m_dispState;						// �ǂ��܂ŕ\�����Ă��邩
};


#endif