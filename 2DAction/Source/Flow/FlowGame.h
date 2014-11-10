/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		ÉQÅ[ÉÄâÊñ ä«óùÉNÉâÉX
 */
/* ====================================================================== */
#ifndef FLOW_GAME
#define FLOW_GAME

#include "FlowBase.h"

class FlowGame : public FlowBase
{

public:

	static FlowBase *Create( const std::string &fileName );

private:

	virtual bool Init();
	virtual void FlowUpdate();

	FlowGame( const std::string &fileName );
	virtual ~FlowGame(void);
};
#endif

