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

#include "FlowMenuBase.h"
#include "Game/Game2DBase.h"
#include "System/Menu/SystemMenuWindow.h"

class ViewScoreMenu;

class FlowViewScore : public FlowMenuBase
{
public:

	static FlowBase *Create( const std::string &fileName );

private:

	virtual bool Init() override;

	FlowViewScore( const std::string &fileName );
	~FlowViewScore(void);

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
	
protected:
	
	virtual bool InitMenu() override;
	virtual void UpdateMenu() override;
	virtual void PadEventCancel() override;

private:

	ViewScoreMenu( const std::string &readMenuJson );
	virtual ~ViewScoreMenu(void);
	
	Common::SAVE_DATA	m_saveData;
};
#endif