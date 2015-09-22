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
#include "Process/FlowProcessControllDescription.h"
#include "Process/FlowProcessInitLastStage.h"

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
	// �Q�[�����ɕ\������X�R�A����
	m_pNumScore = NumberCounter::Create("Number.json");

	// �Q�[��������̂ɕK�v�ȃC���X�^���X�쐬
	GameRegister::CreateInstance();

	const GameManager *pGameManager = GameRegister::GetInstance()->GetManagerGame();
	if( pGameManager->GetStageType() == GameManager::TYPE_TIME ){
		// �c�莞�Ԃ�\������N���X�p��
		m_pStageTimer = StageTimer::CreateStageTimer( pGameManager->GetGameLeftTimeBySec() );
	}

	std::string soundTag = "";
	switch( GameRecorder::GetInstance()->GetGameStateOfProgress() ){
	case GameRecorder::STATE_STAGE01:
	case GameRecorder::STATE_STAGE02:
	case GameRecorder::STATE_STAGE03:
		{
			// �ŏ��̐���
			ControllDescription *pEffectStage = ControllDescription::Create();
			PushStageEffect( pEffectStage );

			soundTag = "stage01";
		}
		break;
	case GameRecorder::STATE_STAGE04:
		soundTag = "stage02";
		break;

	case GameRecorder::STATE_STAGE05:
	case GameRecorder::STATE_STAGE06:
	case GameRecorder::STATE_STAGE07:
		soundTag = "stage02";
		break;

	case GameRecorder::STATE_STAGE08:
		soundTag = "stage03";
		break;

	case GameRecorder::STATE_STAGE09:
	case GameRecorder::STATE_STAGE10:
	case GameRecorder::STATE_STAGE11:
		soundTag = "stage03";
		break;

	case GameRecorder::STATE_STAGE12:
		{
			// �{�X�폀��
			InitLastStage *pEffectStage = InitLastStage::Create();
			PushStageEffect( pEffectStage );

			soundTag = "stage03";
		}
		break;
	}

	// BGM�Đ��J�n
	SoundManager::GetInstance()->PlayBGM( soundTag.c_str() );

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
