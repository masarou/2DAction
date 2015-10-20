/* ====================================================================== */
/**
 * @brief  
 *		�Q�[����ʑ����N���X
 * @note
 *		
 */
/* ====================================================================== */
#include "FlowGame.h"
#include "Game/GameRegister.h"
#include "Game/GameRecorder.h"
#include "Game/Timer/GameStageTimer.h"
#include "Flow/Process/FlowProcessFirstManual.h"
#include "Common/Utility/CommonGameUtility.h"
#include "Process/FlowProcessStageStart.h"

FlowBase *FlowGame::Create( const std::string &fileName )
{
	return NEW FlowGame(fileName);
}

FlowGame::FlowGame( const std::string &fileName )
: FlowBase(fileName)
, m_pNumScore( NULL )
, m_pStageTimer( NULL )
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
	// �Q�[��������̂ɕK�v�ȃC���X�^���X�쐬
	GameRegister::CreateInstance();

	const GameManager *pGameManager = GameRegister::GetInstance()->GetManagerGame();
	if( pGameManager->GetStageType() == GameManager::TYPE_TIME ){
		// �c�莞�Ԃ�\������N���X�p��
		m_pStageTimer = StageTimer::CreateStageTimer( pGameManager->GetGameLeftTimeBySec() );
	}

	std::string soundTag = "";
	std::string readFileScore = "Number.json";
	switch( FlowManager::GetInstance()->GetCurrentFlowKind() ){
	case Common::FLOW_STAGE01:
	case Common::FLOW_STAGE02:
		{
			soundTag = "stage01";
		}
		break;
	case Common::FLOW_STAGE03:
		soundTag = "BossBattle";
		break;

	case Common::FLOW_STAGE04:
	case Common::FLOW_STAGE05:
		soundTag = "stage02";
		break;

	case Common::FLOW_STAGE06:
		soundTag = "BossBattle";
		break;

	case Common::FLOW_STAGE07:
		soundTag = "stage03";
		readFileScore = "NumberWhite.json";	// �w�i�����Ȃ̂Ō��₷����������Score�\��
		break;

	case Common::FLOW_STAGE08:
		soundTag = "stage03";
		readFileScore = "NumberWhite.json";
		break;

	case Common::FLOW_STAGE09:
		soundTag = "BossBattle";
		readFileScore = "NumberWhite.json";
		break;

	case Common::FLOW_STAGE10:
		soundTag = "stageLast";
		readFileScore = "NumberWhite.json";
		break;
	}

	// �Q�[�����ɕ\������X�R�A����
	m_pNumScore = NumberCounter::Create( readFileScore.c_str() );
	if( m_pNumScore ){
		// ���l���ω�����Ƃ���SE�͖炳�Ȃ�
		m_pNumScore->SetCountUpSeInvalidFlag( true );
	}

	// BGM�Đ��J�n
	SoundManager::GetInstance()->PlayBGM( soundTag.c_str() );


	// ���߂ẴQ�[���v���C���ǂ����`�F�b�N
	Common::SAVE_DATA saveData;
	Utility::GetSaveData( saveData );
	if( saveData.m_isFirstGamePlay ){
		// ��������Z�b�g
		FirstManual *pFirstManual = FirstManual::Create( FirstManual::KIND_GAMEPLAY01 );
		PushStageEffect( pFirstManual );
		pFirstManual = FirstManual::Create( FirstManual::KIND_GAMEPLAY02 );
		PushStageEffect( pFirstManual );

		// ���߂ẴQ�[���v���C�t���O��������
		saveData.m_isFirstGamePlay = false;
		Utility::OverWriteSaveData( saveData );
	}

	// �����߂̃X�e�[�W������ʂɏo��
	StageStart *pStageStart = StageStart::Create();
	PushStageEffect( pStageStart );

	return true;
}

void FlowGame::UpdateFlowAfterChildTask()
{
	// ��ʓ��X�R�A�\���̍X�V
	if( m_pNumScore ){
		uint32_t currScore = GameRecorder::GetInstance()->GetScore();
		m_pNumScore->SetValue( currScore );
	}

	const GamePlayer *pPlayer = GameRegister::GetInstance()->GetPlayer();
	const GameManager *pGameMan = GameRegister::GetInstance()->GetManagerGame();
	if( ( pPlayer->GetCurrentLife() == 0 || pGameMan->IsGameOver() )
		&& !IsPlaySpecialEffect() ){

		StartFade("gameend");

		// �I�����_�ł̃��[�U�[���C�t���i�[���Ă���
		const GamePlayer *pPlayer = GameRegister::GetInstance()->GetPlayer();
		uint32_t leftLife = pPlayer->GetCurrentLife();
		float ratio = static_cast<float>(leftLife) / static_cast<float>(pPlayer->GetPlayerLifeMax());
		GameRecorder::GetInstance()->SetUserLifeRatio( ratio );
	}
}
