/* ====================================================================== */
/**
 * @brief  
 *		ゲームクリアー後リンク表示画面
 * @note
 *		
 */
/* ====================================================================== */
#ifndef __FLOW_GAME_CLEAR__
#define __FLOW_GAME_CLEAR__

#include "FlowMenuBase.h"

class FlowGameClear : public FlowMenuBase
{
public:

	static FlowBase *Create( const std::string &fileName );

private:

	virtual bool Init() override;

	FlowGameClear( const std::string &fileName );
	~FlowGameClear(void);
	
	Texture2D			m_addressTex;	// 表示画像データ
};


class ShowAddressMenu : public MenuWindow
{
public:

	static ShowAddressMenu *CreateViewScore2D( const std::string &readMenuJson );
	
protected:
	
	virtual void UpdateMenu() override;
	virtual void PadEventDecide() override;

private:

	ShowAddressMenu( const std::string &readMenuJson );
	virtual ~ShowAddressMenu(void);
};

#endif