/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		ゲーム結果画面クラス
 */
/* ====================================================================== */
#include "FlowExit.h"

FlowBase *FlowExit::Create( const std::string &fileName )
{
	return NEW FlowExit(fileName);
}

FlowExit::FlowExit( const std::string &fileName )
: FlowBase(fileName)
{
	DEBUG_PRINT("FlowExit生成！！\n");
}


FlowExit::~FlowExit(void)
{
	DEBUG_PRINT("FlowExit削除！！\n");
}

bool FlowExit::Init()
{
	PostQuitMessage(0);
	return true;
}

