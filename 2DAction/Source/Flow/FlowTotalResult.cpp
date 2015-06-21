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

void FlowTotalResult::RecordGameResult()
{
	// ����̃v���C�𔽉f�����������L���O�쐬
	Common::SAVE_DATA updateData;
	bool retVal = Utility::GetSaveData( updateData );
	if( retVal ){
		// �����L���O�쐬
		UpdateSortRanking( updateData );

		// �擾�����o�g���|�C���g�𔽉f������
		updateData.m_battlePoint = 10*GameRecorder::GetInstance()->GetItemCount( Common::ITEM_KIND_BATTLE_POINT );
	}
	
	// �Z�[�u�f�[�^�㏑��
	Utility::OverWriteSaveData( updateData );
}


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
, m_dispState(DISP_STAGE01)
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
		"scoreStage01",
		"scoreStage02",
		"scoreStage03",
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
	m_pNumCounter[static_cast<uint32_t>(DISP_STAGE01)]->AddValue( GameRecorder::GetInstance()->GetStageScore( GameRecorder::STATE_STAGE01 ) );

	return true;
}

void TotalResult2D::UpdateMenu()
{
	CallPadEvent();

	// �A�j���J�E���g���I����Ă���Ȃ玟�̃X�e�b�v�ɐi��
	uint32_t index = static_cast<uint32_t>( m_dispState );
	switch(m_dispState){
	case DISP_STAGE01:
		if( !m_pNumCounter[index]->IsPlayCountAnim() ){
			m_dispState = DISP_STAGE02;
			m_pNumCounter[static_cast<uint32_t>(DISP_STAGE02)]->AddValue( GameRecorder::GetInstance()->GetStageScore( GameRecorder::STATE_STAGE02 ) );
		}
		break;
	case DISP_STAGE02:
		if( !m_pNumCounter[index]->IsPlayCountAnim() ){
			m_dispState = DISP_STAGE03;
			m_pNumCounter[static_cast<uint32_t>(DISP_STAGE03)]->AddValue( GameRecorder::GetInstance()->GetStageScore( GameRecorder::STATE_STAGE03 ) );
		}
		break;
	case DISP_STAGE03:
		if( !m_pNumCounter[index]->IsPlayCountAnim() ){
			m_dispState = DISP_TOTAL;
			m_pNumCounter[static_cast<uint32_t>(DISP_TOTAL)]->AddValue( GameRecorder::GetInstance()->GetTotalScore() );
		}
		break;
	case DISP_TOTAL:
		if( !m_pNumCounter[index]->IsPlayCountAnim() ){
			m_dispState = DISP_MAX;
		}
		break;
	case DISP_MAX:

		break;
	}

}

void TotalResult2D::PadEventDecide()
{
	uint32_t index = static_cast<uint32_t>( m_dispState );
	switch(m_dispState){
	case DISP_STAGE01:
		// �J�E���g�A�j���I��
		m_pNumCounter[index]->CountAnimEnd();
		m_dispState = DISP_STAGE02;

		m_pNumCounter[static_cast<uint32_t>(DISP_STAGE02)]->AddValue( GameRecorder::GetInstance()->GetStageScore( GameRecorder::STATE_STAGE02 ) );
		break;
	case DISP_STAGE02:
		// �J�E���g�A�j���I��
		m_pNumCounter[index]->CountAnimEnd();
		m_dispState = DISP_STAGE03;

		m_pNumCounter[static_cast<uint32_t>(DISP_STAGE03)]->AddValue( GameRecorder::GetInstance()->GetStageScore( GameRecorder::STATE_STAGE03 ) );
		break;
	case DISP_STAGE03:
		// �J�E���g�A�j���I��
		m_pNumCounter[index]->CountAnimEnd();
		m_dispState = DISP_TOTAL;

		m_pNumCounter[static_cast<uint32_t>(DISP_TOTAL)]->AddValue( GameRecorder::GetInstance()->GetTotalScore() );
		break;
	case DISP_TOTAL:
		// �J�E���g�A�j���I��
		m_pNumCounter[index]->CountAnimEnd();
		m_dispState = DISP_MAX;
		break;
	case DISP_MAX:
		SetNextFlowStr( "title" );
		break;
	}
}
