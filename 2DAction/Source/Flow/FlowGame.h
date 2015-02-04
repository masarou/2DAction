/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		ゲーム画面管理クラス
 */
/* ====================================================================== */
#ifndef __FLOW_GAME__
#define __FLOW_GAME__

#include "FlowBase.h"

class FlowGame : public FlowBase
{

public:

	static FlowBase *Create( const std::string &fileName );

private:

	virtual bool Init() override;
	virtual void UpdateFlow() override;

	FlowGame( const std::string &fileName );
	virtual ~FlowGame(void);
};
#endif

