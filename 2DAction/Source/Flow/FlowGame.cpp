
/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		�^�C�g����ʊǗ��N���X
 */
/* ====================================================================== */

#include "FlowGame.h"
#include "Game/GameRegister.h"

GamePlayer *player = NULL;

FlowBase *FlowGame::Create(std::string fileName)
{
	return NEW FlowGame(fileName);
}

FlowGame::FlowGame(std::string fileName)
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

	//if(player->IsNext()){
	//	StartFade("proceed");
	//}
}