
/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		Flow�N���X��Factory
 */
/* ====================================================================== */

#include <map>
#include "System/SystemDefine.h"
#include "FlowTable.h"
#include "FlowTitle.h"
#include "FlowGame.h"
#include "FlowResult.h"
#include "FlowRetry.h"
#include "FlowViewScore.h"
#include "FlowExit.h"

typedef FlowBase *(*pFunkRetFlowBase)( const std::string &fileName );

struct FLOW_DATA{
	pFunkRetFlowBase	pCallFunktion;
	std::string			jsonKeyPath;
};

//! �e�t���[��json���̃e�[�u��
static FLOW_DATA s_flowTablse[] = 
{
	{FlowTitle::Create,		"FlowTitle.json"},
	{FlowGame::Create,		"FlowGameMain.json"},
	{FlowResult::Create,	"FlowGameResult.json"},
	{FlowRetry::Create,		"FlowRetry.json"},
	{FlowViewScore::Create,	"FlowScore.json"},
	{FlowExit::Create,		"FlowExit.json"},

};

FlowTable::FlowTable()
{
}


FlowTable::~FlowTable(void)
{
}

//! json�t�@�C���܂ł̃p�X���쐬
std::string FlowTable::CreateFlowFilePath(std::string fileName)
{
	std::string filePath = JSON_FLOW_PATH;
	filePath += fileName;
	return filePath;
}

FlowBase *FlowTable::CreateFlow(const char* filePath)
{
	std::string path = JSON_FLOW_PATH;
	path += filePath;

	//! �e�[�u������Y���̃t�@�C����T���A�΂ɂȂ��Ă���֐����Ă�
	for(uint32_t i = 0; i < NUMBEROF(s_flowTablse); ++i){
		std::string tmp = CreateFlowFilePath(s_flowTablse[i].jsonKeyPath);
		if(strcmp(path.c_str(), tmp.c_str()) == 0){
			return (*s_flowTablse[i].pCallFunktion)(path.c_str());
		}
	}

	return NULL;
}