/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		ゲーム履歴表示クラス
 */
/* ====================================================================== */
#include "FlowViewScore.h"

FlowBase *FlowViewScore::Create( const std::string &fileName )
{
	return NEW FlowViewScore(fileName);
}

FlowViewScore::FlowViewScore( const std::string &fileName )
: FlowBase(fileName)
{
	DEBUG_PRINT("FlowViewScore生成！！\n");
}


FlowViewScore::~FlowViewScore(void)
{
	DEBUG_PRINT("FlowResult削除！！\n");
}

bool FlowViewScore::Init()
{
	return true;
}

/* ================================================ */
/**
 * @brief	パッド操作関数
 */
/* ================================================ */
void FlowViewScore::PadEventCancel()
{
	StartFade("return");
}
