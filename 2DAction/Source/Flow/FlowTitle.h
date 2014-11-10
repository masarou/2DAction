
/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		�^�C�g����ʊǗ��N���X
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

	Game2DBase			*m_title2D;		// �^�C�g���ꖇ�G�N���X
	TEX_DRAW_INFO		m_titleInfo;	// �^�C�g���ꖇ�G���

};
#endif

