/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		ゲーム画面管理クラス
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

	virtual bool Init() override;
	virtual void UpdateFlow() override;

	FlowGame( const std::string &fileName );
	virtual ~FlowGame(void);

	uint32_t	m_gameTimer;		// ゲームのプレイ時間

};
#endif

