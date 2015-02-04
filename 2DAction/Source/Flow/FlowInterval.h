/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		ゲーム終了後リトライ確認画面クラス
 */
/* ====================================================================== */
#ifndef __FLOW_RETRY__
#define __FLOW_RETRY__

#include "FlowBase.h"
#include "Game/Game2DBase.h"
#include "System/Task/SystemTaskUnit.h"

class Interval2D;

class FlowInterval : public FlowBase 
{
public:

	static FlowBase *Create( const std::string &fileName);

protected:
	
	virtual bool Init() override;
	virtual void PadEventDecide() override;

private:

	std::string GetNextFadeStr();

	FlowInterval( const std::string &fileName );
	~FlowInterval(void);
	
	Interval2D	*m_pRetryTex;
};

/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		リトライ選択肢クラス
 */
/* ====================================================================== */
class Interval2D : public TaskUnit, InputWatcher
{
public:
	
	enum{
		SELECT_NEXT,
		SELECT_TITLE,

		SELECT_RETRY_MAX,
	};

	static Interval2D *CreateRetry2D();

	const uint32_t &GetSelectedNo() const{ return m_selectNo; }

protected:
	
	virtual bool Init() override;
	virtual void Update() override;
	virtual void DrawUpdate() override;		// 描画更新
	
	virtual void PadEventUp() override;
	virtual void PadEventDown() override;
	virtual void PadEventRight() override;
	virtual void PadEventLeft() override;

private:

	Interval2D();
	virtual ~Interval2D(void);
	
	Texture2D			m_textureRetry;	// タイトル一枚絵
	
	uint32_t			m_selectNo;
	Game2DBase			*m_pTexChoiceArray[SELECT_RETRY_MAX];	// 選択肢
	Game2DBase			*m_pTexChoiceBGArray[SELECT_RETRY_MAX];	// 選択肢背景
	TEX_DRAW_INFO		m_texInfo;								// 選択肢描画情報
};

#endif