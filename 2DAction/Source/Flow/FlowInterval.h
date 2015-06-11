/* ====================================================================== */
/**
 * @brief  
 *		ゲーム終了後リトライ、次に進むか確認する画面
 * @note
 *		
 */
/* ====================================================================== */
#ifndef __FLOW_INTERVAL__
#define __FLOW_INTERVAL__

#include "FlowBase.h"
#include "Game/Game2DBase.h"
#include "System/Task/SystemTaskUnit.h"
#include "System/Menu/SystemMenuWindow.h"

class IntervalMenu;

class FlowInterval : public FlowBase 
{
public:

	static FlowBase *Create( const std::string &fileName);

protected:
	
	virtual bool Init() override;
	virtual void UpdateFlowAfterChildTask();

private:

	std::string GetNextFadeStr();


	FlowInterval( const std::string &fileName );
	virtual ~FlowInterval(void);

	IntervalMenu	*m_pMenuWindow;
};

/* ====================================================================== */
/**
 * @brief  
 *		FlowIntervalの表示物中身
 * @note
 *		
 */
/* ====================================================================== */
class IntervalMenu : public MenuWindow
{
public:
	
	enum{
		SELECT_NEXT,
		SELECT_TITLE,

		SELECT_RETRY_MAX,
	};

	static IntervalMenu *CreateIntervalMenu( const std::string &readMenuJson );

	const uint32_t &GetSelectedNo() const{ return m_selectNo; }

	const std::string GetNextFlowStr() const{ return m_nextFlow; }

protected:
	
	virtual bool InitMenu() override;
	virtual void UpdateMenu() override;
	virtual void PadEventDecide() override;
	virtual void PadEventRight() override;
	virtual void PadEventLeft() override;

private:

	enum{
		SELECT_NEXT_GAME,
		SELECT_TO_TITLE,

		SELECT_MAX,
	};

	IntervalMenu( const std::string &readMenuJson );
	virtual ~IntervalMenu(void);

	uint32_t			m_selectNo;
	std::string			m_nextFlow;
};

#endif