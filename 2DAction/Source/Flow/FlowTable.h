
/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		Flow�N���X��Factory
 */
/* ====================================================================== */
#ifndef FLOW_TABLE
#define FLOW_TABLE

#include "FlowBase.h"

class FlowTable
{
public:

	static FlowBase *CreateFlow(const char* filePath);

	//! json�t�@�C���܂ł̃p�X���쐬
	static std::string CreateFlowFilePath(std::string fileName);

private:

	FlowTable();
	virtual ~FlowTable(void);

};
#endif

