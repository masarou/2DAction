
/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		タイトル画面管理クラス
 */
/* ====================================================================== */

#include "FlowTitle.h"

FlowBase *FlowTitle::Create(std::string fileName)
{
	return NEW FlowTitle(fileName);
}

FlowTitle::FlowTitle(std::string fileName)
	: FlowBase(fileName)
{
	DEBUG_PRINT("FlowTitle生成！！\n");
}


FlowTitle::~FlowTitle(void)
{
	DEBUG_PRINT("FlowTitle削除！！\n");
}

bool FlowTitle::Init()
{
	return true;
}

void FlowTitle::FlowUpdate()
{
	CallPadEvent();
	ChildUpdate();
}

void FlowTitle::PadEventDecide()
{
	StartFade("proceed");
}