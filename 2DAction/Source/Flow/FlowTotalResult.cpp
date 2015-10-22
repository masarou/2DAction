/* ====================================================================== */
/**
 * @brief  
 *		�Q�[�����ʉ�ʃN���X
 * @note
 *		�e�X�e�[�W�̃X�R�A��\�����āA���Z���o�����
 */
/* ====================================================================== */

#include <fstream>
#include <algorithm>
#include "FlowTotalResult.h"
#include "Game/GameRecorder.h"
#include "Common/Utility/CommonGameUtility.h"
#include "System/Draw2D/SystemDraw2DResource.h"

FlowBase *FlowTotalResult::Create( const std::string &fileName )
{
	return NEW FlowTotalResult(fileName);
}

FlowTotalResult::FlowTotalResult( const std::string &fileName )
: FlowMenuBase(fileName)
{
	DEBUG_PRINT("FlowTotalResult�����I�I\n");
}


FlowTotalResult::~FlowTotalResult(void)
{
	DEBUG_PRINT("FlowTotalResult�폜�I�I\n");
}

bool FlowTotalResult::Init()
{
	// BGM�Đ��J�n
	SoundManager::GetInstance()->PlayBGM( "interval" );

	// �w�i�ꖇ�G�쐬
	m_pMenuWindow = TotalResult2D::CreateTotalResult2D( "MenuGameResult.json" );
	return true;
}

bool FlowTotalResult::FinishFlow()
{
	// ����̃Q�[���̌��ʔ��f
	RecordGameResult();

	// �풓���ŏ��������K�v�Ȃ��̃��Z�b�g
	Utility::GameInitALL();
	return true;
}

/* ====================================================================== */
/**
 * @brief �Z�[�u�f�[�^�̍X�V
 */
/* ====================================================================== */
void FlowTotalResult::RecordGameResult()
{
	// ����̃v���C�𔽉f�����������L���O�쐬
	Common::SAVE_DATA updateData;
	bool retVal = Utility::GetSaveData( updateData );
	if( retVal ){

		// �ŏ�����v���C�������ǂ����`�F�b�N
		//if( !GameRecorder::GetInstance()->IsContinue() ){
			// �����L���O�쐬
			UpdateSortRanking( updateData );
		//}

		// �擾�����o�g���|�C���g�𔽉f������
		updateData.m_battlePoint = GameRecorder::GetInstance()->GetClearBattlePointAll();
	}
	
	// �Z�[�u�f�[�^�㏑��
	Utility::OverWriteSaveData( updateData );
}

/* ====================================================================== */
/**
 * @brief �����̃����L���O���\�[�g
 */
/* ====================================================================== */
bool sortScore(const uint32_t &left, const uint32_t &rRight)
{
	return left > rRight;
}
void FlowTotalResult::UpdateSortRanking( Common::SAVE_DATA &scoreData )
{
	// ���܂ł̃X�R�A��push
	std::vector<uint32_t> ranking;
	for( uint32_t i = 0; i < NUMBEROF(scoreData.m_scoreRanking) ; ++i ){
		ranking.push_back(scoreData.m_scoreRanking[i]);
	}

	// ����̃X�R�A��push
	uint32_t scoreCurr = GameRecorder::GetInstance()->GetTotalScore();
	ranking.push_back(scoreCurr);

	// ���ёւ�
	std::sort( ranking.begin(), ranking.end(), sortScore);
	
	for( uint32_t i = 0; i < NUMBEROF(scoreData.m_scoreRanking) ; ++i ){
		scoreData.m_scoreRanking[i] = ranking.at(i);
	}
}

/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		�\������X�R�A���̂���
 */
/* ====================================================================== */
TotalResult2D *TotalResult2D::CreateTotalResult2D( const std::string &readMenuJson )
{
	return NEW TotalResult2D( readMenuJson );
}

TotalResult2D::TotalResult2D( const std::string &readMenuJson )
: MenuWindow( readMenuJson )
, m_dispState(DISP_SCENE01)
{
	for( uint32_t i = 0; i < DISP_MAX ; ++i ){
		m_pNumCounter[i] = NULL;
	}
}

TotalResult2D::~TotalResult2D(void)
{
}

bool TotalResult2D::InitMenu()
{
	static const char *s_counterName[DISP_MAX] = {
		"scoreScene01",
		"scoreScene02",
		"scoreScene03",
		"scoreTotal",
	};

	// �����J�E���^�Z�b�g
	for( uint32_t i = 0; i < NUMBEROF(s_counterName) ; ++i ){
		m_pNumCounter[i] = GetPartsCounter( s_counterName[i] );
		if( m_pNumCounter[i] == NULL ){
			DEBUG_ASSERT( 0, "�J�E���^�p�[�c��������Ȃ�");
			return true;
		}
	}

	// �G��|���ē������_���Z�b�g
	uint32_t sceneScore = GameRecorder::GetInstance()->GetStageScore( GameRecorder::STATE_STAGE01 )
						+ GameRecorder::GetInstance()->GetStageScore( GameRecorder::STATE_STAGE02 )
						+ GameRecorder::GetInstance()->GetStageScore( GameRecorder::STATE_STAGE03 );
	m_pNumCounter[static_cast<uint32_t>(DISP_SCENE01)]->AddValue( sceneScore );

	return true;
}

void TotalResult2D::UpdateMenu()
{
	CallPadEvent();

	// �A�j���J�E���g���I����Ă���Ȃ玟�̃X�e�b�v�ɐi��
	PartsCounter *pCounter = NULL;
	uint32_t index = static_cast<uint32_t>( m_dispState );
	switch(m_dispState){
	case DISP_SCENE01:
	case DISP_SCENE02:
	case DISP_SCENE03:
	case DISP_TOTAL:
		pCounter = m_pNumCounter[index];
		break;
	case DISP_MAX:

		break;
	}

	if( pCounter && !pCounter->IsPlayCountAnim() ){
		ChangeDispNextState();
	}

}

void TotalResult2D::PadEventDecide()
{
	if( ChangeDispNextState() ){

		if( GameRecorder::GetInstance()->IsClearAll() ){
			// �S���N���A�[�Ȃ�A�h���X�\����ʂ�
			SetNextFlowStr( "gameaddress" );
		}
		else{
			SetNextFlowStr( "title" );
		}
		// ����SE�炷
		SoundManager::GetInstance()->PlaySE("Decide");
	}
}

/* ====================================================================== */
/**
 * @brief ��ʂ̕\���X�V���ڂ����̂��̂�
 */
/* ====================================================================== */
bool TotalResult2D::ChangeDispNextState()
{
	if( m_dispState == DISP_MAX ){
		return true;
	}

	uint32_t index = static_cast<uint32_t>( m_dispState );
	m_pNumCounter[index]->CountAnimEnd();
	switch(m_dispState){
	case DISP_SCENE01:
		{
			m_dispState = DISP_SCENE02;
			uint32_t sceneScore = GameRecorder::GetInstance()->GetStageScore( GameRecorder::STATE_STAGE04 )
								+ GameRecorder::GetInstance()->GetStageScore( GameRecorder::STATE_STAGE05 )
								+ GameRecorder::GetInstance()->GetStageScore( GameRecorder::STATE_STAGE06 );
			m_pNumCounter[static_cast<uint32_t>(DISP_SCENE02)]->AddValue( sceneScore );
		}
		break;
	case DISP_SCENE02:
		{
			m_dispState = DISP_SCENE03;		
			uint32_t sceneScore = GameRecorder::GetInstance()->GetStageScore( GameRecorder::STATE_STAGE07 )
								+ GameRecorder::GetInstance()->GetStageScore( GameRecorder::STATE_STAGE08 )
								+ GameRecorder::GetInstance()->GetStageScore( GameRecorder::STATE_STAGE09 )
								+ GameRecorder::GetInstance()->GetStageScore( GameRecorder::STATE_STAGE10 );
			m_pNumCounter[static_cast<uint32_t>(DISP_SCENE03)]->AddValue( sceneScore );
		}
		break;
	case DISP_SCENE03:
		m_dispState = DISP_TOTAL;
		m_pNumCounter[static_cast<uint32_t>(DISP_TOTAL)]->AddValue( GameRecorder::GetInstance()->GetTotalScore() );
		break;
	case DISP_TOTAL:
		// �\������
		m_dispState = DISP_MAX;
		break;
	case DISP_MAX:
		return true;
		break;
	}
	return false;
}

