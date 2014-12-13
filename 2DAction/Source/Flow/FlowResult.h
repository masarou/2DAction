/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		ゲーム結果画面クラス
 */
/* ====================================================================== */
#ifndef FLOW_RESULT
#define FLOW_RESULT

#include "FlowBase.h"
#include "Game/Game2DBase.h"
#include "Common/CmnNumberCounter.h"
#include "System/Task/SystemTaskUnit.h"

class Result2D;

class FlowResult : public FlowBase
{
public:

	static FlowBase *Create( const std::string &fileName );

private:

	virtual bool Init() override;
	virtual void PadEventDecide() override;

	// 引数に今回のプレイデータを反映したランキングをセット
	void UpdateSortRanking( Common::SAVE_SCORE &scoreData );
	
	FlowResult( const std::string &fileName );
	~FlowResult(void);

	Result2D	*m_pResultTex;
};


/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		タイトル選択肢クラス
 */
/* ====================================================================== */
class Result2D : public TaskUnit, InputWatcher
{
public:

	static Result2D *CreateResult2D();

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

	Result2D();
	virtual ~Result2D(void);

	DISP_STATE			m_dispState;		// どこまで表示しているか
	Texture2D			m_textureResult;	// 背景一枚絵
	TEX_DRAW_INFO		m_numberInfo;		// 描画関係情報
	
	NumberCounter		*m_pNumCounterResult;		// スコア表示
	NumberCounter		*m_pNumCounterBonus;		// スコア表示
	NumberCounter		*m_pNumCounterTotal;		// スコア表示
};


#endif