
/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		FlowクラスのFactory
 */
/* ====================================================================== */
#ifndef __FLOW_TABLE__
#define __FLOW_TABLE__

#include "Common/CommonDefine.h"
#include "FlowBase.h"

class FlowTable
{
public:

	static FlowBase *CreateFlow(const char* filePath);

	// jsonファイルまでのパスを作成
	static std::string CreateFlowFilePath( std::string fileName );
	// Flow名からFlowの種類を取得
	static Common::GAME_FLOW GetGameFlowKind( std::string fileName );

private:

	FlowTable();
	virtual ~FlowTable(void);

};
#endif

