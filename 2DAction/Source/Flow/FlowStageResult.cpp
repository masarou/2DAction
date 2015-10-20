/* ====================================================================== */
/**
 * @brief  
 *		�Q�[���X�e�[�W���ʉ�ʃN���X
 * @note
 *		
 */
/* ====================================================================== */

#include <fstream>
#include <algorithm>
#include "FlowStageResult.h"
#include "FlowManager.h"
#include "Game/GameRecorder.h"
#include "Common/Utility/CommonGameUtility.h"
#include "System/Draw2D/SystemDraw2DResource.h"

FlowBase *FlowStageResult::Create( const std::string &fileName )
{
	return NEW FlowStageResult(fileName);
}

FlowStageResult::FlowStageResult( const std::string &fileName )
: FlowMenuBase( fileName )
{
	DEBUG_PRINT("FlowStageResult�����I�I\n");
}


FlowStageResult::~FlowStageResult(void)
{
	DEBUG_PRINT("FlowStageResult�폜�I�I\n");
}

bool FlowStageResult::Init()
{
	// �w�i�ꖇ�G�쐬
	if( GameRecorder::GetInstance()->IsUserClear() ){
		m_pMenuWindow = ResultStageMenu::CreateResultStageMenu( "MenuStageResult.json" );
	}
	else{
		m_pMenuWindow = ResultStageMenu::CreateResultStageMenu( "MenuStageResultGameOver.json" );
	}
	return true;
}

void FlowStageResult::UpdateFlowAfterChildTask()
{

}

/* ====================================================================== */
/**
 * @brief  
 *			�X�e�[�W���ʂ̒��g�J�E���^�N���X
 * @note
 *		
 */
/* ====================================================================== */
ResultStageMenu *ResultStageMenu::CreateResultStageMenu( const std::string &readMenuJson )
{
	return NEW ResultStageMenu( readMenuJson );
}

ResultStageMenu::ResultStageMenu(  const std::string &readMenuJson  )
: MenuWindow( readMenuJson )
, m_dispState(DISP_RESULT)
{
}

ResultStageMenu::~ResultStageMenu(void)
{
}

bool ResultStageMenu::InitMenu()
{
	if( FlowManager::GetInstance()->GetPreFlowKind() != Common::FLOW_STAGE10 ){
		// BGM�Đ��J�n
		SoundManager::GetInstance()->PlayBGM( "interval" );
	}

	// �G��|���ē������_���Z�b�g
	PartsCounter *pCounter = GetPartsCounter( "result" );
	if( pCounter ){
		pCounter->AddValue( GameRecorder::GetInstance()->GetScore() );
	}

	return true;
}

void ResultStageMenu::UpdateMenu()
{
	CallPadEvent();

	// �A�j���J�E���g���I����Ă���Ȃ玟�̃X�e�b�v�ɐi��
	PartsCounter *pCounter = NULL;
	switch(m_dispState){
	case DISP_RESULT:
		{
			pCounter = GetPartsCounter( "result" );
		}
		break;
	case DISP_BONUS:
		{
			pCounter = GetPartsCounter( "bonus" );
		}
		break;
	case DISP_TOTAL:
		{
			pCounter = GetPartsCounter( "total" );
		}
		break;
	case DISP_BATTLEPOINT:
		
		{
			pCounter = GetPartsCounter( "battlepoint" );
		}
		break;
	case DISP_ALL:

		break;
	}

	if( pCounter && !pCounter->IsPlayCountAnim() ){
		ChangeDispStateNext();
	}
}

void ResultStageMenu::PadEventDecide()
{
	if( ChangeDispStateNext() ){
		// ���ɐi��
		GameRecorder *pRecorder = GameRecorder::GetInstance();
		if( !pRecorder ){
			DEBUG_ASSERT( 0, "GameRecorder is NULL");
			return;
		}
		
		// �v���C���[���C�t��0�ɂȂ����̂ŃQ�[���I�[�o�[
		if( !GameRecorder::GetInstance()->IsUserClear() ){
			SetNextFlowStr( "totalresult" );
			return;
		}

		// �X�e�[�W3,6���N���A������N���A�t���O�𗧂Ă�
		Common::SAVE_DATA saveData;
		Utility::GetSaveData( saveData );

		// ���̃X�e�[�Wor��ʂ�
		switch( FlowManager::GetInstance()->GetPreFlowKind() ){
		default:
			DEBUG_ASSERT( 0, "�z��O�̃t���[" );
			// �Ƃ肠�����^�C�g����
			SetNextFlowStr( "title" );
			break;
		case Common::FLOW_STAGE01:
			SetNextFlowStr( "nextgame02" );
			break;
		case Common::FLOW_STAGE02:
			SetNextFlowStr( "nextgame03" );
			break;
		case Common::FLOW_STAGE03:
			SetNextFlowStr( "nextgame04" );
			saveData.m_isClearPhase01 = true;
			break;
		case Common::FLOW_STAGE04:
			SetNextFlowStr( "nextgame05" );
			break;
		case Common::FLOW_STAGE05:
			SetNextFlowStr( "nextgame06" );
			break;
		case Common::FLOW_STAGE06:
			SetNextFlowStr( "nextgame07" );
			saveData.m_isClearPhase02 = true;
			break;
		case Common::FLOW_STAGE07:
			SetNextFlowStr( "nextgame08" );
			break;
		case Common::FLOW_STAGE08:
			SetNextFlowStr( "nextgame09" );
			break;
		case Common::FLOW_STAGE09:
			SetNextFlowStr( "nextgame10" );
			break;
		case Common::FLOW_STAGE10:
			SetNextFlowStr( "totalresult" );	// ���ׂẴX�e�[�W�I��
			break;
		}

		
		Utility::OverWriteSaveData( saveData );

		// ����SE�炷
		SoundManager::GetInstance()->PlaySE("Decide");
	}
}

uint32_t ResultStageMenu::GetStageClearBonus() const
{
	// �{�[�i�X�Z�o���@
	// �R���{���~���[�U�[�̎c�胉�C�t = Bonus

	uint32_t retVal = 0;
	uint32_t stageComboMax = GameRecorder::GetInstance()->GetMaxComboNumOfStage();

	enum SCORE_STAGE {
		SCORE_01 = 100,
		SCORE_02 = 250,
		SCORE_03 = 400,
		SCORE_04 = 600,
		SCORE_05 = 1000,
	};
	SCORE_STAGE scoreStage = SCORE_01;
	for( uint32_t i = 0; i < stageComboMax ; ++i ){
		switch( scoreStage ){
		case SCORE_01:
			retVal += SCORE_01;
			if( i > 20 ){
				scoreStage = SCORE_02;
			}
			break;
		case SCORE_02:
			retVal += SCORE_02;
			if( i > 50 ){
				scoreStage = SCORE_03;
			}
			break;
		case SCORE_03:
			retVal += SCORE_03;
			if( i > 80 ){
				scoreStage = SCORE_04;
			}
			break;
		case SCORE_04:
			retVal += SCORE_04;
			if( i > 100 ){
				scoreStage = SCORE_05;
			}
			break;
		case SCORE_05:
			retVal += SCORE_05;
			break;
		}
	}
	// �c�X�R�A��������
	retVal = static_cast<uint32_t>( retVal*GameRecorder::GetInstance()->GetUserLifeRatio() );
	return retVal;
}

bool ResultStageMenu::ChangeDispStateNext()
{
	switch(m_dispState){
	case DISP_RESULT:
		{
			// �J�E���g�A�j���I��
			PartsCounter *pCounterResult = GetPartsCounter( "result" );
			if( pCounterResult ){
				pCounterResult->CountAnimEnd();
			}

			PartsCounter *pCounterBonus = GetPartsCounter( "bonus" );
			if( pCounterBonus ){
				pCounterBonus->AddValue( GetStageClearBonus() );
				m_dispState = DISP_BONUS;
			}
			else{
				DEBUG_ASSERT( 0, "�ړI�̃p�[�c���Ȃ��I" );
			}
		}
		break;
	case DISP_BONUS:
		{
			// �J�E���g�A�j���I��
			PartsCounter *pCounterResult = GetPartsCounter( "result" );
			PartsCounter *pCounterBonus = GetPartsCounter( "bonus" );
			if( pCounterBonus ){
				pCounterBonus->CountAnimEnd();
			}

			uint32_t totalValue = 0;
			if( pCounterResult && pCounterBonus ){
				totalValue = pCounterResult->GetValue() + pCounterBonus->GetValue();
			}

			PartsCounter *pCounterTotal = GetPartsCounter( "total" );
			if( pCounterTotal ){
				pCounterTotal->AddValue( totalValue );
				m_dispState = DISP_TOTAL;
			}
			else{
				DEBUG_ASSERT( 0, "�ړI�̃p�[�c���Ȃ��I" );
			}
		}
		break;
	case DISP_TOTAL:
		{
			// �J�E���g�A�j���I��
			PartsCounter *pCounterTotal = GetPartsCounter( "total" );
			if( pCounterTotal ){
				pCounterTotal->CountAnimEnd();
			}

			PartsCounter *pCounterBP = GetPartsCounter( "battlepoint" );
			if( pCounterBP ){
				pCounterBP->AddValue( GameRecorder::GetInstance()->GetClearBattlePoint( pCounterTotal->GetValue() ) );
			}

			// battlePoint�̕\����
			m_dispState = DISP_BATTLEPOINT;
		}
		break;
	case DISP_BATTLEPOINT:
		{
			// �J�E���g�A�j���I��
			PartsCounter *pCounterBP = GetPartsCounter( "battlepoint" );
			if( pCounterBP ){
				pCounterBP->CountAnimEnd();
			}

			// �����ŋ��߂��{�[�i�X���X�e�[�W�X�R�A�ɉ��Z
			PartsCounter *pCounterBonus = GetPartsCounter( "bonus" );
			if( pCounterBonus ){
				GameRecorder::GetInstance()->AddScoreBonus( pCounterBonus->GetValue() );
			}
			m_dispState = DISP_ALL;
		}
		break;
	case DISP_ALL:
		return true;
		break;
	}
	return false;
}