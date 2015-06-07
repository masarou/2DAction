/* ====================================================================== */
/**
 * @brief  
 *		ゲーム履歴表示クラス
 * @note
 *		
 */
/* ====================================================================== */
#ifndef __FLOW_VIEW_SCORE__
#define __FLOW_VIEW_SCORE__

#include "FlowBase.h"
#include "Game/Game2DBase.h"
#include "Common/CmnNumberCounter.h"
#include "System/Menu/SystemMenuWindow.h"

class ViewScoreMenu;

class FlowViewScore : public FlowBase
{
public:

	static FlowBase *Create( const std::string &fileName );

private:

	virtual bool Init() override;
	virtual void UpdateFlowAfterChildTask();

	FlowViewScore( const std::string &fileName );
	~FlowViewScore(void);
	

	ViewScoreMenu	*m_pMenuWindow;
};


/* ====================================================================== */
/**
 * @brief  
 *		表示する数字カウンタ
 * @note
 *		
 */
/* ====================================================================== */
class ViewScoreMenu : public MenuWindow
{
public:

	static ViewScoreMenu *CreateViewScore2D( const std::string &readMenuJson );
	
	const std::string GetNextFlowStr(){ return m_nextFlow; }

protected:
	
	virtual bool InitMenu() override;
	virtual void UpdateMenu() override;
	virtual void PadEventCancel() override;

private:

	ViewScoreMenu( const std::string &readMenuJson );
	virtual ~ViewScoreMenu(void);
	
	Common::SAVE_DATA	m_saveData;
	std::string			m_nextFlow;
};
#endif