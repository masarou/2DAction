/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		ÉQÅ[ÉÄåãâ âÊñ ÉNÉâÉX
 */
/* ====================================================================== */
#ifndef FLOW_EXIT
#define FLOW_EXIT

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