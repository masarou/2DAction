/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		�Q�[�����ʉ�ʃN���X
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
	DEBUG_PRINT("FlowResult�����I�I\n");
}


FlowResult::~FlowResult(void)
{
	DEBUG_PRINT("FlowResult�폜�I�I\n");
}

bool FlowResult::Init()
{
	return true;
}

/* ================================================ */
/**
 * @brief	�p�b�h����֐�
 */
/* ================================================ */
void FlowResult::PadEventDecide()
{
	StartFade("proceed");
}
