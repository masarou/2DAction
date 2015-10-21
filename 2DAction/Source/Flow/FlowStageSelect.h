/* ====================================================================== */
/**
 * @brief  
 *		�Q�[���J�n�O�A���[�U�[�̊�{�\��UP���
 * @note
 *		
 */
/* ====================================================================== */
#ifndef __FLOW_STAGE_SELECT__
#define __FLOW_STAGE_SELECT__

#include "FlowMenuBase.h"
#include "Game/Game2DBase.h"
#include "System/Task/SystemTaskUnit.h"
#include "System/Menu/SystemMenuWindow.h"

class StageSelectMenu;

class FlowStageSelect : public FlowMenuBase
{
public:

	static FlowBase *Create( const std::string &fileName );

protected:
	
	virtual bool Init() override;

private:

	FlowStageSelect( const std::string &fileName );
	~FlowStageSelect(void);

};

/* ====================================================================== */
/**
 * @brief  
 *		FlowStageSelect�̕\�������g
 * @note
 *		
 */
/* ====================================================================== */
class StageSelectMenu : public MenuWindow
{
public:

	enum SELECT_STAGE{
		SEELCT_STAGE01,
		SEELCT_STAGE02,
		SEELCT_STAGE03,

		SELECT_MAX,
	};

	static StageSelectMenu *CreateStageSelectMenu( const std::string &fileName );

protected:
	
	virtual bool InitMenu() override;
	virtual bool DieMainMenu() override;
	virtual void UpdateMenu() override;
	virtual void PadEventDecide() override;
	virtual void PadEventRight() override;
	virtual void PadEventLeft() override;

private:

	StageSelectMenu( const std::string &fileName );
	virtual ~StageSelectMenu(void);

	// �`�悷�鋭�����ڐ����X�V
	void ChangeDispState( const SELECT_STAGE &stage );

	// ���ڂ��Ƃ̐������擾
	std::string GetExplanationStr( const SELECT_STAGE &kind );

	SELECT_STAGE	m_selectStageKind;
	SELECT_STAGE	m_selectStageMax;	// �I���ł���ő�X�e�[�W
	std::string		m_explanationStr;

};

#endif