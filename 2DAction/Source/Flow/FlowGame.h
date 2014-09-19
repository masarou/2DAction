
/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		タイトル画面管理クラス
 */
/* ====================================================================== */
#ifndef FLOW_GAME
#define FLOW_GAME

#include "FlowBase.h"

class FlowGame : public FlowBase
{

public:

	static FlowBase *Create(std::string fileName);

private:
	virtual bool Init();
	virtual void FlowUpdate();

	FlowGame(std::string fileName);
	virtual ~FlowGame(void);
};
#endif

