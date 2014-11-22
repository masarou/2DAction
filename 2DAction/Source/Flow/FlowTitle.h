
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
class Title2D : public TaskUnit
{
public:

	static Title2D *CreateTitle2D();

protected:

	virtual void DrawUpdate() override;		// 描画更新

private:

	Title2D();
	virtual ~Title2D(void);

	Game2DBase			*m_title2D;		// タイトル一枚絵
	TEX_DRAW_INFO		m_titleInfo;	// タイトル一枚絵情報
};
#endif

