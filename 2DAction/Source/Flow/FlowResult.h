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

	void UpdateSortRanking( Common::SAVE_SCORE &scoreData );

	bool CheckSaveData( Common::SAVE_SCORE &saveData );

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
	
	enum{
		SELECT_START,
		SELECT_SCORE,
		SELECT_EXIT,

		SELECT_MAX,
	};

	static Result2D *CreateResult2D();

protected:
	
	virtual bool Init() override;
	virtual void Update() override;
	virtual void DrawUpdate() override;		// 描画更新

private:

	Result2D();
	virtual ~Result2D(void);
	
	Texture2D			m_textureResult;	// タイトル一枚絵

	NumberCounter		*m_pNumCounter;		// スコア表示
	TEX_DRAW_INFO		m_numberInfo;		// 描画関係情報
};


#endif