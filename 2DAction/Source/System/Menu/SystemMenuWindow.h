/* ====================================================================== */
/**
 * @brief  Menu基底クラス
 *
 * @note
 *		主にJsonを呼んで描画のみを行う
		padの検知やそれに対する挙動等は派生クラスで行う
 */
/* ====================================================================== */
#ifndef __MENU_WINDOW__
#define __MENU_WINDOW__

#include "Game/Game2DBase.h"
#include "System/Task/SystemTaskUnit.h"
#include "System/Input/SystemInputWatcher.h"

class MenuParts;

class MenuWindow : public TaskUnit, public InputWatcher
{

public:

protected:

	MenuWindow( const std::string &readMenuJson );
	virtual ~MenuWindow(void);
	
	// 継承禁止!!!
	virtual bool Init() override;
	virtual bool DieMain() override;
	virtual void DrawUpdate() override;
	
	// 派生先専用
	virtual bool InitMenu(){ return true; }

	// 継承先各Menu画面からアニメイベントセット
	void SetAnim( const std::string &partsStr, const std::string &animStr );

private:
	
	std::string	m_readMenuFile;
	MenuParts	*m_pMainParts;

};
#endif // __MENU_WINDOW__