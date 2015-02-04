/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		ゲーム結果画面クラス
 */
/* ====================================================================== */
#ifndef __FLOW_TOTAL_RESULT__
#define __FLOW_TOTAL_RESULT__

#include "FlowBase.h"
#include "Game/Game2DBase.h"
#include "Common/CmnNumberCounter.h"
#include "System/Task/SystemTaskUnit.h"

class TotalResult2D;

class FlowTotalResult : public FlowBase
{
public:

	static FlowBase *Create( const std::string &fileName );

private:

	virtual bool Init() override;
	virtual bool FinishFlow() override;
	virtual void PadEventDecide() override;

	// 引数に今回のプレイデータを反映したランキングをセット
	void UpdateSortRanking( Common::SAVE_SCORE &scoreData );
	
	FlowTotalResult( const std::string &fileName );
	~FlowTotalResult(void);

	TotalResult2D	*m_pResultTex;
};


/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		タイトル選択肢クラス
 */
/* ====================================================================== */
class TotalResult2D : public TaskUnit, InputWatcher
{
public:

	static TotalResult2D *CreateTotalResult2D();

	bool ProceedNext(){ return m_dispState == DISP_ALL ? true : false ;}		// 全て表示し終えて次に進んでいいかどうか

protected:
	
	virtual bool Init() override;
	virtual void Update() override;
	virtual void DrawUpdate() override;		// 描画更新
	virtual void PadEventDecide() override;

private:

	enum DISP_STATE{
		DISP_RESULT,
		DISP_BONUS,
		DISP_TOTAL,
		DISP_ALL,

		DISP_MAX,
	};

	TotalResult2D();
	virtual ~TotalResult2D(void);

	DISP_STATE			m_dispState;		// どこまで表示しているか
	Texture2D			m_textureResult;	// 背景一枚絵
	TEX_DRAW_INFO		m_numberInfo;		// 描画関係情報
	
	NumberCounter		*m_pNumCounterResult;		// スコア表示
	NumberCounter		*m_pNumCounterBonus;		// スコア表示
	NumberCounter		*m_pNumCounterTotal;		// スコア表示
};


#endif