
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
#include "Game/GameRecorder.h"

FlowBase *FlowGame::Create( const std::string &fileName )
{
	return NEW FlowGame(fileName);
}

FlowGame::FlowGame( const std::string &fileName )
: FlowBase(fileName)
, m_pNumCounter( NULL )
{
	DEBUG_PRINT("FlowGame�����I�I\n");
}


FlowGame::~FlowGame(void)
{
	// �Q�[������̃N���X���
	GameRegister::GetInstance()->DeleteRegister();

	DEBUG_PRINT("FlowGame�폜�I�I\n");
}

bool FlowGame::Init()
{
	// �Q�[�����ɕ\������X�R�A����
	m_pNumCounter = NumberCounter::Create("number.json");

	// �Q�[��������̂ɕK�v�ȃC���X�^���X�쐬
	GameRegister::CreateInstance();

	return true;
}

void FlowGame::UpdateFlowAfterChildTask()
{
	// ��ʓ��X�R�A�\���̍X�V
	if( m_pNumCounter ){
		uint32_t currScore = GameRecorder::GetInstance()->GetScore();
		m_pNumCounter->SetValue( currScore );
	}

	const GamePlayer *pPlayer = GameRegister::GetInstance()->GetPlayer();
	const GameManager *pGameMan = GameRegister::GetInstance()->GetManagerGame();
	if( pPlayer->GetCurrentLife() == 0 /*|| pGameMan->IsGameOver()*/ ){
		StartFade("gameend");
	}
}
