/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		��ʂɉ�������\������t���[�N���X�̃x�[�X
 */
/* ====================================================================== */
#ifndef __FLOW_MENU_BASE__
#define __FLOW_MENU_BASE__

#include "System/SystemDefine.h"
#include "Flow/FlowBase.h"
#include "System/Menu/SystemMenuWindow.h"

class FlowMenuBase : public FlowBase
{

protected:

	FlowMenuBase( const std::string &fileName );
	void CheckNextFlow() override;
	MenuWindow	*m_pMenuWindow;

public:

	virtual ~FlowMenuBase(void);

};

#endif