
/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		Flow�N���X��Factory
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

	// json�t�@�C���܂ł̃p�X���쐬
	static std::string CreateFlowFilePath( std::string fileName );
	// Flow������Flow�̎�ނ��擾
	static Common::GAME_FLOW GetGameFlowKind( std::string fileName );

private:

	FlowTable();
	virtual ~FlowTable(void);

};
#endif

