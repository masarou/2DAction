/* ====================================================================== */
/**
 * @brief  
 *		タイトル画面管理クラス
 * @note
 *		
 */
/* ====================================================================== */
#ifndef __FLOW_TITLE__
#define __FLOW_TITLE__

#include "Flow/FlowMenuBase.h"
#include "Game/Game2DBase.h"
#include "System/Task/SystemTaskUnit.h"

class FlowTitle : public FlowMenuBase
{
public:

	static FlowBase *Create( const std::string &fileName );

protected:

	virtual bool Init() override;
	virtual void UpdateFlowAfterChildTask();

private:

	FlowTitle( const std::string &fileName );
	virtual ~FlowTitle(void);

};

/* ====================================================================== */
/**
 * @brief  
 *		タイトル選択肢クラス
 * @note
 *		
 */
/* ====================================================================== */
class TitleMenu : public MenuWindow
{
public:

	static TitleMenu *Create( const std::string &readMenuJson );

protected:

	virtual bool InitMenu() override;
	
	virtual void UpdateMenu() override;

	virtual void PadEventDecide() override;
	virtual void PadEventUp() override;
	virtual void PadEventDown() override;
	virtual void PadEventRight() override;
	virtual void PadEventLeft() override;

private:

	enum{
		SELECT_START,
		SELECT_SCORE,
		SELECT_EXIT,

		SELECT_MAX,
	};

	void SetChoiceSelect( uint32_t choiceIndex );

	TitleMenu( const std::string &readMenuJson );
	virtual ~TitleMenu(void);
	
};
#endif

