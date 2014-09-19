
/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		FlowクラスのFactory
 */
/* ====================================================================== */
#ifndef FLOW_TABLE
#define FLOW_TABLE

#include "FlowBase.h"

class FlowTable
{
public:

	static FlowBase *CreateFlow(const char* filePath);

	//! jsonファイルまでのパスを作成
	static std::string CreateFlowFilePath(std::string fileName);

private:

	FlowTable();
	virtual ~FlowTable(void);

};
#endif

