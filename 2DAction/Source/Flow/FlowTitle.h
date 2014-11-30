
/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		タイトル画面管理クラス
 */
/* ====================================================================== */
#ifndef FLOW_TITLE
#define FLOW_TITLE

#include "FlowBase.h"
#include "Game/Game2DBase.h"
#include "System/Task/SystemTaskUnit.h"

class Title2D;

class FlowTitle : public FlowBase
{
public:

	static FlowBase *Create( const std::string &fileName );

protected:

	virtual bool Init() override;
	virtual bool Finish() override;
	virtual void PadEventDecide() override;

private:

	FlowTitle( const std::string &fileName );
	virtual ~FlowTitle(void);

	Title2D	*m_pTitleTex;

};

/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		タイトル一枚絵クラス
 */
/* ====================================================================== */
class Title2D : public TaskUnit, InputWatcher
{
public:

	static Title2D *CreateTitle2D();

protected:
	
	virtual bool Init() override;
	virtual void Update() override;
	virtual void DrawUpdate() override;		// 描画更新

	virtual void PadEventUp() override;
	virtual void PadEventDown() override;
	virtual void PadEventRight() override;
	virtual void PadEventLeft() override;

private:

	enum{
		SELECT_START,
		SELECT_SCORE,
		SELECT_EXIT,

		SELECT_MAX,
	};

	Title2D();
	virtual ~Title2D(void);
	
	Texture2D			m_textureTitle;	// タイトル一枚絵
	
	uint32_t			m_selectNo;
	Game2DBase			*m_pTexChoice;	// 選択肢
	Game2DBase			*m_pTexChoiceBG;// 選択肢背景
	TEX_DRAW_INFO		m_texInfo;		// 選択肢描画情報
};
#endif

