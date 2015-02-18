/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		ゲーム結果画面クラス
 */
/* ====================================================================== */
#ifndef __FLOW_STAGE_RESULT__
#define __FLOW_STAGE_RESULT__

#include "FlowBase.h"
#include "Game/Game2DBase.h"
#include "Common/CmnNumberCounter.h"
#include "System/Task/SystemTaskUnit.h"

class Result2D;

class FlowStageResult : public FlowBase
{
public:

	static FlowBase *Create( const std::string &fileName );

private:

	virtual bool Init() override;
	virtual void PadEventDecide() override;
	
	FlowStageResult( const std::string &fileName );
	~FlowStageResult(void);

	Result2D	*m_pResultTex;
};


/* ====================================================================== */
/**
 * @brief  
 *			外枠+カウンタクラス
 * @note
 *		
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
	const math::Vector2 GetPartsPos( const std::string name ) const;
	const Common::PARTS_INFO &GetPartsInfo( const std::string name ) const;

	DISP_STATE			m_dispState;						// どこまで表示しているか
	Texture2D			m_textureResult;					// 画面フレーム
	Texture2D			m_textureBG;						// 背景一枚絵
	TEX_DRAW_INFO		m_numberInfo;						// 描画関係情報
	
	NumberCounter		*m_pNumCounterResult;				// スコア表示
	NumberCounter		*m_pNumCounterBonus;				// スコア表示
	NumberCounter		*m_pNumCounterTotal;				// スコア表示

	std::map< std::string, Common::PARTS_INFO >	m_partsMap;	// パーツマップ
};


#endif