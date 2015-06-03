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

#include "FlowBase.h"
#include "Game/Game2DBase.h"
#include "System/Task/SystemTaskUnit.h"
#include "System/Menu/SystemMenuWindow.h"

class TitleMenu;

class FlowTitle : public FlowBase
{
public:

	static FlowBase *Create( const std::string &fileName );

protected:

	virtual bool Init() override;
	virtual void UpdateFlowAfterChildTask();

private:

	FlowTitle( const std::string &fileName );
	virtual ~FlowTitle(void);

	TitleMenu	*m_pMenuWindow;
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

	const uint32_t &GetSelectedNo(){ return m_selectNo; }

	const std::string GetNextFlowStr(){ return m_nextFlow; }

protected:

	virtual bool InitMenu() override;
	
	virtual void Update() override;

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

	TitleMenu( const std::string &readMenuJson );
	virtual ~TitleMenu(void);
	
	uint32_t			m_selectNo;
	std::string			m_nextFlow;
};
#endif

