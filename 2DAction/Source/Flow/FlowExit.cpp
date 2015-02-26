/* ====================================================================== */
/**
 * @brief  
 *		�Q�[���I���N���X
 * @note
 *		
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
	DEBUG_PRINT("FlowExit�����I�I\n");
}


FlowExit::~FlowExit(void)
{
	DEBUG_PRINT("FlowExit�폜�I�I\n");
}

bool FlowExit::Init()
{
	PostQuitMessage(0);
	return true;
}

