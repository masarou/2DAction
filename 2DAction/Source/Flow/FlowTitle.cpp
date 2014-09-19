
/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		�^�C�g����ʊǗ��N���X
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
	DEBUG_PRINT("FlowTitle�����I�I\n");
}


FlowTitle::~FlowTitle(void)
{
	DEBUG_PRINT("FlowTitle�폜�I�I\n");
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