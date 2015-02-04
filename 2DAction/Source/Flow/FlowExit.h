/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		�Q�[�����ʉ�ʃN���X
 */
/* ====================================================================== */
#ifndef __FLOW_EXIT__
#define __FLOW_EXIT__

#include "FlowBase.h"

class FlowExit : public FlowBase
{
public:

	static FlowBase *Create( const std::string &fileName );

private:

	virtual bool Init() override;

	FlowExit( const std::string &fileName );
	~FlowExit(void);

};
#endif