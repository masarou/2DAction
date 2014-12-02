/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		ゲーム履歴表示クラス
 */
/* ====================================================================== */
#ifndef FLOW_VIEW_SCORE
#define FLOW_VIEW_SCORE

#include "FlowBase.h"

class FlowViewScore : public FlowBase
{
public:

	static FlowBase *Create( const std::string &fileName );

private:

	virtual bool Init() override;
	virtual void PadEventCancel() override;

	FlowViewScore( const std::string &fileName );
	~FlowViewScore(void);

};
#endif