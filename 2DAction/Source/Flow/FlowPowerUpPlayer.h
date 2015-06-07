/* ====================================================================== */
/**
 * @brief  
 *		ゲーム開始前、ユーザーの基本能力UP画面
 * @note
 *		
 */
/* ====================================================================== */
#ifndef __FLOW_POWER_UP__
#define __FLOW_POWER_UP__

#include "FlowBase.h"
#include "Game/Game2DBase.h"
#include "System/Task/SystemTaskUnit.h"
#include "System/Menu/SystemMenuWindow.h"

class PowerUpMenu;

class FlowPowerUpPlayer : public FlowBase
{
public:

	static FlowBase *Create( const std::string &fileName );

protected:
	
	virtual bool Init() override;
	virtual void PadEventDecide() override;
	virtual void UpdateFlowPreChildTask() override;

private:

	std::string GetNextFadeStr();

	FlowPowerUpPlayer( const std::string &fileName );
	~FlowPowerUpPlayer(void);
	
	PowerUpMenu		*m_pMenuWindow;
};

/* ====================================================================== */
/**
 * @brief  
 *		FlowPowerUpPlayerの表示物中身
 * @note
 *		
 */
/* ====================================================================== */
class PowerUpMenu : public MenuWindow
{
public:

	static PowerUpMenu *CreatePowerUp2D( const std::string &fileName );

	// 描画する強化項目説明更新
	void ChangeDispState( const Common::PLAYER_BASE_STATE &kind );

protected:
	
	virtual bool InitMenu() override;
	virtual void UpdateMenu() override;

private:

	PowerUpMenu( const std::string &fileName );
	virtual ~PowerUpMenu(void);

	std::string		m_nextFlow;

};

#endif