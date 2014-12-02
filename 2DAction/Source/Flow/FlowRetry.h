/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		ゲーム終了後リトライ確認画面クラス
 */
/* ====================================================================== */
#ifndef FLOW_RETRY
#define FLOW_RETRY

#include "FlowBase.h"
#include "Game/Game2DBase.h"
#include "System/Task/SystemTaskUnit.h"

class Retry2D;

class FlowRetry : public FlowBase 
{
public:

	static FlowBase *Create( const std::string &fileName);

private:

	virtual bool Init() override;
	virtual bool Finish() override;
	virtual void PadEventDecide() override;

	FlowRetry( const std::string &fileName );
	~FlowRetry(void);
	
	Retry2D	*m_pRetryTex;
};

/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		リトライ選択肢クラス
 */
/* ====================================================================== */
class Retry2D : public TaskUnit, InputWatcher
{
public:
	
	enum{
		SELECT_RETRY,
		SELECT_TITLE,

		SELECT_RETRY_MAX,
	};

	static Retry2D *CreateRetry2D();

	uint32_t GetSelectedNo(){ return m_selectNo; }

protected:
	
	virtual bool Init() override;
	virtual void Update() override;
	virtual void DrawUpdate() override;		// 描画更新
	
	virtual void PadEventUp() override;
	virtual void PadEventDown() override;
	virtual void PadEventRight() override;
	virtual void PadEventLeft() override;

private:

	Retry2D();
	virtual ~Retry2D(void);
	
	Texture2D			m_textureRetry;	// タイトル一枚絵
	
	uint32_t			m_selectNo;
	Game2DBase			*m_pTexChoiceArray[SELECT_RETRY_MAX];	// 選択肢
	Game2DBase			*m_pTexChoiceBGArray[SELECT_RETRY_MAX];	// 選択肢背景
	TEX_DRAW_INFO		m_texInfo;								// 選択肢描画情報
};

#endif