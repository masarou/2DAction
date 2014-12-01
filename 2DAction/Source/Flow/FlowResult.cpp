/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		ゲーム結果画面クラス
 */
/* ====================================================================== */
#include "FlowResult.h"

FlowBase *FlowResult::Create( const std::string &fileName )
{
	return NEW FlowResult(fileName);
}

FlowResult::FlowResult( const std::string &fileName )
: FlowBase(fileName)
{
	DEBUG_PRINT("FlowResult生成！！\n");
}


FlowResult::~FlowResult(void)
{
	DEBUG_PRINT("FlowResult削除！！\n");
}

bool FlowResult::Init()
{
	return true;
}

/* ================================================ */
/**
 * @brief	パッド操作関数
 */
/* ================================================ */
void FlowResult::PadEventDecide()
{
	StartFade("proceed");
}
