/* ====================================================================== */
/**
 * @brief  
 *		�Q�[���J�n�O�A���[�U�[�̊�{�\��UP���
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
 *		FlowPowerUpPlayer�̕\�������g
 * @note
 *		
 */
/* ====================================================================== */
class PowerUpMenu : public MenuWindow
{
public:

	static PowerUpMenu *CreatePowerUp2D( const std::string &fileName );

	// �`�悷�鋭�����ڐ����X�V
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