
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
#include "System/Input/SystemInputWatcher.h"

class FlowTitle : public FlowBase, public InputWatcher 
{
public:

	static FlowBase *Create( const std::string &fileName );

protected:

	virtual bool Init() override;
	virtual bool Finish() override;
	virtual void FlowUpdate() override;
	virtual void PadEventDecide() override;

private:

	FlowTitle( const std::string &fileName );
	virtual ~FlowTitle(void);

	Game2DBase			*m_title2D;		// タイトル一枚絵クラス
	TEX_DRAW_INFO		m_titleInfo;	// タイトル一枚絵情報

};
#endif

