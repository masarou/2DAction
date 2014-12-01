/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		�Q�[������\���N���X
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
	DEBUG_PRINT("FlowViewScore�����I�I\n");
}


FlowViewScore::~FlowViewScore(void)
{
	DEBUG_PRINT("FlowResult�폜�I�I\n");
}

bool FlowViewScore::Init()
{
	return true;
}

/* ================================================ */
/**
 * @brief	�p�b�h����֐�
 */
/* ================================================ */
void FlowViewScore::PadEventCancel()
{
	StartFade("return");
}
