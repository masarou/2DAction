
/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		�Q�[����ʊǗ��N���X
 */
/* ====================================================================== */

#include "FlowGame.h"
#include "Game/GameRegister.h"

FlowBase *FlowGame::Create( const std::string &fileName )
{
	return NEW FlowGame(fileName);
}

FlowGame::FlowGame( const std::string &fileName )
	: FlowBase(fileName)
{
	DEBUG_PRINT("FlowGame�����I�I\n");
}


FlowGame::~FlowGame(void)
{
	// �Q�[������̃N���X���
	GameRegister *pRegister = GameRegister::GetInstance();
	SAFE_DELETE( pRegister );

	DEBUG_PRINT("FlowGame�폜�I�I\n");
}

bool FlowGame::Init()
{
	// �Q�[��������̂ɕK�v�ȃC���X�^���X�쐬
	GameRegister::CreateInstance();

	return true;
}

void FlowGame::FlowUpdate()
{
	ChildUpdate();

	if( GameRegister::GetInstance()->GetPlayer()->GetCurrentLife() == 0 ){
		StartFade("gameend");
	}
}