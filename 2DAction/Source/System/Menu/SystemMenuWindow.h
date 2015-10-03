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
#include "SystemMenuPartsCounter.h"
#include "System/Task/SystemTaskUnit.h"
#include "System/Input/SystemInputWatcher.h"
#include "System/Draw2D/SystemDraw2DDefine.h"

class MenuParts;

class MenuWindow : public TaskUnit, public InputWatcher
{

public:

	const uint32_t &GetSelectedNo() const{ return m_selectNo; }
	const std::string GetNextFlowStr() const{ return m_nextFlow; }

protected:

	MenuWindow( const std::string &readMenuJson, const Common::PRIORITY &priority = Common::PRIORITY_NORMAL, const math::Vector2 &partsPos = math::Vector2() );
	virtual ~MenuWindow(void);
	
	// 継承禁止!!!
	virtual bool Init() override;
	virtual bool DieMain() override;
	virtual void Update() override;
	virtual void DrawUpdate() override;
	
	// 派生先専用
	virtual bool InitMenu(){ return true; }
	virtual bool DieMainMenu(){ return true; }
	virtual void UpdateMenu(){};
	virtual void DrawUpdateMenu(){};

	// 継承先各Menu画面からアニメイベントセット
	void SetAnim( const std::string &partsStr, const std::string &animStr );
	void SetAnim( const std::string &partsStr, const uint32_t &num );
	void SetSelectNum( const uint32_t &selNum ){ m_selectNo = selNum; }
	void SetNextFlowStr( const std::string &nextFlowStr ){ m_nextFlow = nextFlowStr; }

	// 各パーツクラス取得
	MenuParts		*GetParts( const std::string &partsStr );
	PartsCounter	*GetPartsCounter( const std::string &partsStr );

private:
	
	std::string	m_readMenuFile;
	MenuParts	*m_pMainParts;
	
	math::Vector2		m_posMainParts;	// 画面全体の位置情報
	Common::PRIORITY	m_priority;		// 描画優先順位
	uint32_t			m_selectNo;		// 選択肢番号(画面によっては使用しないこともあり)
	std::string			m_nextFlow;		// 次の遷移先

};
#endif // __MENU_WINDOW__