/* ====================================================================== */
/**
 * @brief  
 *		ゲーム結果画面クラス
 * @note
 *		各ステージのスコアを表示して、合算を出す画面
 */
/* ====================================================================== */
#ifndef __FLOW_TOTAL_RESULT__
#define __FLOW_TOTAL_RESULT__

#include "FlowMenuBase.h"
#include "Game/Game2DBase.h"
#include "Common/CmnNumberCounter.h"
#include "System/Task/SystemTaskUnit.h"

class PartsCounter;

class FlowTotalResult : public FlowMenuBase
{
public:

	static FlowBase *Create( const std::string &fileName );

private:

	virtual bool Init() override;
	virtual bool FinishFlow() override;

	// 引数に今回のプレイデータを反映したランキングをセット
	void RecordGameResult();
	void UpdateSortRanking( Common::SAVE_DATA &scoreData );
	
	FlowTotalResult( const std::string &fileName );
	~FlowTotalResult(void);

};


/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		表示するスコアそのもの
 */
/* ====================================================================== */
class TotalResult2D : public MenuWindow
{
public:

	static TotalResult2D *CreateTotalResult2D( const std::string &readMenuJson );

	bool ProceedNext(){ return m_dispState == DISP_MAX ? true : false ;}		// 全て表示し終えて次に進んでいいかどうか

protected:
	
	virtual bool InitMenu() override;
	virtual void UpdateMenu() override;
	virtual void PadEventDecide() override;

private:

	enum DISP_STATE{
		DISP_STAGE01,
		DISP_STAGE02,
		DISP_STAGE03,
		DISP_TOTAL,

		DISP_MAX,
	};

	TotalResult2D( const std::string &readMenuJson );
	virtual ~TotalResult2D(void);

	DISP_STATE			m_dispState;						// どこまで表示しているか
	PartsCounter		*m_pNumCounter[DISP_MAX];			// スコア表示
};


#endif