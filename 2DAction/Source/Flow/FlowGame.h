/* ====================================================================== */
/**
 * @brief  
 *		ÉQÅ[ÉÄâÊñ ëçäáÉNÉâÉX
 * @note
 *		
 */
/* ====================================================================== */
#ifndef __FLOW_GAME__
#define __FLOW_GAME__

#include "FlowBase.h"
#include "Common/CmnNumberCounter.h"

class StageTimer;

class FlowGame : public FlowBase
{

public:

	static FlowBase *Create( const std::string &fileName );

private:

	virtual bool Init() override;
	virtual void UpdateFlowAfterChildTask() override;

	FlowGame( const std::string &fileName );
	virtual ~FlowGame(void);

	NumberCounter	*m_pNumScore;
	StageTimer		*m_pStageTimer;
};
#endif

