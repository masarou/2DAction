/* ====================================================================== */
/**
 * @brief  
 *		ゲーム開始前、ユーザーの基本能力UP画面
 * @note
 *		
 */
/* ====================================================================== */
#ifndef __FLOW_STAGE_SELECT__
#define __FLOW_STAGE_SELECT__

#include "FlowMenuBase.h"
#include "Game/Game2DBase.h"
#include "System/Task/SystemTaskUnit.h"
#include "System/Menu/SystemMenuWindow.h"

class StageSelectMenu;

class FlowStageSelect : public FlowMenuBase
{
public:

	static FlowBase *Create( const std::string &fileName );

protected:
	
	virtual bool Init() override;

private:

	FlowStageSelect( const std::string &fileName );
	~FlowStageSelect(void);

};

/* ====================================================================== */
/**
 * @brief  
 *		FlowStageSelectの表示物中身
 * @note
 *		
 */
/* ====================================================================== */
class StageSelectMenu : public MenuWindow
{
public:

	enum SELECT_STAGE{
		SEELCT_STAGE01,
		SEELCT_STAGE02,
		SEELCT_STAGE03,

		SELECT_MAX,
	};

	static StageSelectMenu *CreateStageSelectMenu( const std::string &fileName );

protected:
	
	virtual bool InitMenu() override;
	virtual bool DieMainMenu() override;
	virtual void UpdateMenu() override;
	virtual void PadEventDecide() override;
	virtual void PadEventRight() override;
	virtual void PadEventLeft() override;

private:

	StageSelectMenu( const std::string &fileName );
	virtual ~StageSelectMenu(void);

	// 描画する強化項目説明更新
	void ChangeDispState( const SELECT_STAGE &stage );

	// 項目ごとの説明文取得
	std::string GetExplanationStr( const SELECT_STAGE &kind );

	SELECT_STAGE	m_selectStageKind;
	SELECT_STAGE	m_selectStageMax;	// 選択できる最大ステージ
	std::string		m_explanationStr;

};

#endif