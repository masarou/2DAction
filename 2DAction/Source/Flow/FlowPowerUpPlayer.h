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

#include "FlowMenuBase.h"
#include "Game/Game2DBase.h"
#include "System/Task/SystemTaskUnit.h"
#include "System/Menu/SystemMenuWindow.h"

class PowerUpMenu;

class FlowPowerUpPlayer : public FlowMenuBase
{
public:

	static FlowBase *Create( const std::string &fileName );

protected:
	
	virtual bool Init() override;

private:

	FlowPowerUpPlayer( const std::string &fileName );
	~FlowPowerUpPlayer(void);

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

	enum{
		SEELCT_ITEM,
		SELECT_GAME_START,

		SELECT_MAX,
	};

	static PowerUpMenu *CreatePowerUp2D( const std::string &fileName );

protected:
	
	virtual bool InitMenu() override;
	virtual bool DieMainMenu() override;
	virtual void UpdateMenu() override;

	virtual void PadEventDecide() override;
	virtual void PadEventUp() override;
	virtual void PadEventDown() override;
	virtual void PadEventRight() override;
	virtual void PadEventLeft() override;
	virtual void PadEventCommand( const uint32_t &commandKind ) override;

private:

	PowerUpMenu( const std::string &fileName );
	virtual ~PowerUpMenu(void);

	// 描画する強化項目説明更新
	void ChangeDispState( const Common::PLAYER_BASE_STATE &kind );

	// カーソル移動の処理まとめ
	void UpdateCursorMove();

	// 項目ごとの説明文取得
	std::string GetExplanationStr( const Common::PLAYER_BASE_STATE &kind );

	// 次のレベルまでのポイントを取得
	uint32_t GetPointToNextLevel( const Common::PLAYER_BASE_STATE &kind, uint32_t currLevel );

	bool						m_isProceedEnd;	// 決定進行で画面が終了したかどうか

	Common::SAVE_DATA			m_loadData;
	Common::PLAYER_BASE_STATE	m_selectStateKind;

	std::string					m_explanationStr;
};

#endif