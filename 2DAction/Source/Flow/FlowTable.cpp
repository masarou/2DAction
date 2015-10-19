
/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		FlowクラスのFactory
 */
/* ====================================================================== */

#include <map>
#include "System/SystemDefine.h"
#include "Common/CommonDefine.h"
#include "FlowTable.h"
#include "FlowTitle.h"
#include "FlowViewScore.h"
#include "FlowGame.h"
#include "FlowStageResult.h"
#include "FlowStageSelect.h"
#include "FlowInterval.h"
#include "FlowTotalResult.h"
#include "FlowExit.h"
#include "FlowPowerUpPlayer.h"
#include "FlowGameClearShowAddress.h"

typedef FlowBase *(*pFunkRetFlowBase)( const std::string &fileName );

struct FLOW_DATA{
	pFunkRetFlowBase	pCallFunktion;
	std::string			jsonKeyPath;
	Common::GAME_FLOW	flowKind;
};

//! 各フローとjson名のテーブル
static FLOW_DATA s_flowTablse[] = 
{
	{	FlowTitle::Create,			"FlowTitle.json",				Common::FLOW_TITLE},
	{	FlowViewScore::Create,		"FlowScore.json",				Common::FLOW_SCORE},
	{	FlowPowerUpPlayer::Create,	"FlowPowerUpPlayer.json",		Common::FLOW_POWER_UP},
	{	FlowStageSelect::Create,	"FlowStageSelect.json",			Common::FLOW_STAGE_SEL},
	{	FlowGame::Create,			"FlowGameStage01.json",			Common::FLOW_STAGE01},
	{	FlowGame::Create,			"FlowGameStage02.json",			Common::FLOW_STAGE02},
	{	FlowGame::Create,			"FlowGameStage03.json",			Common::FLOW_STAGE03},
	{	FlowGame::Create,			"FlowGameStage04.json",			Common::FLOW_STAGE04},
	{	FlowGame::Create,			"FlowGameStage05.json",			Common::FLOW_STAGE05},
	{	FlowGame::Create,			"FlowGameStage06.json",			Common::FLOW_STAGE06},
	{	FlowGame::Create,			"FlowGameStage07.json",			Common::FLOW_STAGE07},
	{	FlowGame::Create,			"FlowGameStage08.json",			Common::FLOW_STAGE08},
	{	FlowGame::Create,			"FlowGameStage09.json",			Common::FLOW_STAGE09},
	{	FlowGame::Create,			"FlowGameStage10.json",			Common::FLOW_STAGE10},
	{	FlowStageResult::Create,	"FlowStageResult.json",			Common::FLOW_RESULT},
	{	FlowInterval::Create,		"FlowInterval.json",			Common::FLOW_INTERVAL},
	{	FlowTotalResult::Create,	"FlowTotalResult.json",			Common::FLOW_RESULT_TOTAL},
	{	FlowGameClear::Create,		"FlowGameEndShowAddress.json",	Common::FLOW_GAME_CLEAR},
	{	FlowExit::Create,			"FlowExit.json",				Common::FLOW_EXIT},

};

FlowTable::FlowTable()
{
}


FlowTable::~FlowTable(void)
{
}

//! jsonファイルまでのパスを作成
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

	//! テーブルから該当のファイルを探し、対になっている関数を呼ぶ
	for(uint32_t i = 0; i < NUMBEROF(s_flowTablse); ++i){
		std::string tmp = CreateFlowFilePath(s_flowTablse[i].jsonKeyPath);
		if(strcmp(path.c_str(), tmp.c_str()) == 0){
			return (*s_flowTablse[i].pCallFunktion)(path.c_str());
		}
	}

	DEBUG_ASSERT( 0, "次のフローが見つからない" );
	return NULL;
}

Common::GAME_FLOW FlowTable::GetGameFlowKind( std::string fileName )
{
	Common::GAME_FLOW retKind = Common::FLOW_MAX;
	for(uint32_t i = 0; i < NUMBEROF(s_flowTablse); ++i){
		std::string tmp = s_flowTablse[i].jsonKeyPath;
		if(strcmp(fileName.c_str(), tmp.c_str()) == 0){
			retKind = s_flowTablse[i].flowKind;
			break;
		}
	}
	return retKind;
}

