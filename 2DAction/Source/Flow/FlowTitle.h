
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
#include "System/Input/SystemInputWatcher.h"

class FlowTitle : public FlowBase, public InputWatcher 
{
public:

	static FlowBase *Create(std::string fileName);

protected:

	virtual bool Init();
	virtual void FlowUpdate();
	virtual void PadEventDecide();

private:

	FlowTitle(std::string fileName);
	virtual ~FlowTitle(void);



};
#endif

