/* ====================================================================== */
/**
 * @brief  
 *		ゲームステージ結果画面クラス
 * @note
 *		
 */
/* ====================================================================== */
#ifndef __FLOW_STAGE_RESULT__
#define __FLOW_STAGE_RESULT__

#include "FlowMenuBase.h"
#include "Game/Game2DBase.h"
#include "System/Task/SystemTaskUnit.h"
#include "System/Menu/SystemMenuWindow.h"

class FlowStageResult : public FlowMenuBase
{
public:

	static FlowBase *Create( const std::string &fileName );

private:

	virtual bool Init() override;
	virtual void UpdateFlowAfterChildTask();
	
	FlowStageResult( const std::string &fileName );
	~FlowStageResult(void);

};


/* ====================================================================== */
/**
 * @brief  
 *			ステージ結果の中身カウンタクラス
 * @note
 *		
 */
/* ====================================================================== */
class ResultStageMenu : public MenuWindow
{
public:

	static ResultStageMenu *CreateResultStageMenu( const std::string &readMenuJson );

	bool ProceedNext(){ return m_dispState == DISP_ALL ? true : false ;}		// 全て表示し終えて次に進んでいいかどうか

protected:
	
	virtual bool InitMenu() override;
	virtual void UpdateMenu() override;
	virtual void PadEventDecide() override;

private:

	enum DISP_STATE{
		DISP_RESULT,
		DISP_BONUS,
		DISP_TOTAL,
		DISP_BATTLEPOINT,
		DISP_ALL,

		DISP_MAX,
	};

	ResultStageMenu( const std::string &readMenuJson );
	virtual ~ResultStageMenu(void);
	uint32_t GetStageClearBonus() const;

	// カウント項目を次へ(trueで全部表示しきった)
	bool	ChangeDispStateNext();

	DISP_STATE			m_dispState;						// どこまで表示しているか
};


#endif