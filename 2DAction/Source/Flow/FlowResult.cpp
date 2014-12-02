/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		�Q�[�����ʉ�ʃN���X
 */
/* ====================================================================== */

#include <fstream>
#include <algorithm>
#include "FlowResult.h"
#include "Game/GameScoreRecorder.h"

FlowBase *FlowResult::Create( const std::string &fileName )
{
	return NEW FlowResult(fileName);
}

FlowResult::FlowResult( const std::string &fileName )
: FlowBase(fileName)
, m_pResultTex(NULL)
{
	DEBUG_PRINT("FlowResult�����I�I\n");
}


FlowResult::~FlowResult(void)
{
	DEBUG_PRINT("FlowResult�폜�I�I\n");
}

bool FlowResult::Init()
{
	// �w�i�ꖇ�G�쐬
	m_pResultTex = Result2D::CreateResult2D();

	// ����̃v���C�𔽉f�����������L���O�쐬
	Common::SAVE_SCORE scoreLog;
	bool retVal = CheckSaveData( scoreLog );
	if( retVal ){
		UpdateSortRanking( scoreLog );
	}

	// �v���C���O�����o��
	FILE *fpWrite = fopen( "playLog.dat", "wb" );
	if( fpWrite == NULL ){
		return 0;
	}
	fwrite( &scoreLog, sizeof(scoreLog), 1, fpWrite );
	fclose( fpWrite );

	return true;
}

/* ================================================ */
/**
 * @brief	�p�b�h����֐�
 */
/* ================================================ */
void FlowResult::PadEventDecide()
{
	StartFade("proceed");
}

bool sortScore(const uint32_t &left, const uint32_t &rRight)
{
	return left > rRight;
}

void FlowResult::UpdateSortRanking( Common::SAVE_SCORE &scoreData )
{
	// ���܂ł̃X�R�A��push
	std::vector<uint32_t> ranking;
	for( uint32_t i = 0; i < NUMBEROF(scoreData.m_scoreTimeAttack) ; ++i ){
		ranking.push_back(scoreData.m_scoreTimeAttack[i]);
	}

	// ����̃X�R�A��push
	uint32_t scoreCurr = ScoreRecorder::GetInstance()->GetScore();
	ranking.push_back(scoreCurr);

	// ���ёւ�
	std::sort( ranking.begin(), ranking.end(), sortScore);
	
	for( uint32_t i = 0; i < NUMBEROF(scoreData.m_scoreTimeAttack) ; ++i ){
		scoreData.m_scoreTimeAttack[i] = ranking.at(i);
	}
}

bool FlowResult::CheckSaveData( Common::SAVE_SCORE &saveData )
{
	// �t�@�C�����Ȃ��ꍇ������̂ň�x�J���č쐬���Ă���
	FILE *fpCheck = fopen( "playLog.dat", "r" );
	if( !fpCheck ){
		// �t�@�C�����Ȃ��ꍇ������̂ň�x�J���č쐬���Ă���
		FILE *fpCreate = fopen( "playLog.dat", "a" );
		fclose( fpCreate );

		// �f�t�H���g�̒l���l�߂Ă���
		Common::SAVE_SCORE scoreLog = {
			{ 1000, 500, 300, 100, 0},
			{ 1000, 500, 300, 100, 0},
		};
		FILE *fpWriteDef = fopen( "playLog.dat", "wb" );
		if( fpWriteDef == NULL ){
			return false;
		}
		fwrite( &scoreLog, sizeof(scoreLog), 1, fpWriteDef );
		fclose( fpWriteDef );
	}
	else{
		// �t�@�C���͑��݂��Ă���
		fclose( fpCheck );
	}

	// �v���C���O�ǂݍ���
	FILE *fpRead = fopen( "playLog.dat", "rb" );
	if( fpRead == NULL ){
		return 0;
	}
	fread( &saveData, sizeof(saveData), 1, fpRead );
	fclose( fpRead );

	return true;
}


/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		�^�C�g���ꖇ�G�N���X
 */
/* ====================================================================== */
Result2D *Result2D::CreateResult2D()
{
	return NEW Result2D();
}

Result2D::Result2D()
: TaskUnit("Result2D")
, m_pNumCounter( NULL )
{
	m_textureResult.Init();
	m_textureResult.m_pTex2D = NEW Game2DBase("title.json");
	m_textureResult.m_texInfo.Init();
	m_textureResult.m_texInfo.m_pos.x = WINDOW_WIDTH / 2.0f;
	m_textureResult.m_texInfo.m_pos.y = WINDOW_HEIGHT / 2.0f;
	m_textureResult.m_texInfo.m_usePlayerOffset = false;
	m_textureResult.m_pTex2D->SetDrawInfo(m_textureResult.m_texInfo);

	// �����\���p�摜���
	m_numberInfo.Init();
	m_numberInfo.m_pos.x = WINDOW_WIDTH / 2.0f;
	m_numberInfo.m_pos.y = WINDOW_HEIGHT / 2.0f;
	m_numberInfo.m_usePlayerOffset = false;
}

Result2D::~Result2D(void)
{
	m_textureResult.DeleteAndInit();
}

bool Result2D::Init()
{
	m_pNumCounter = NumberCounter::Create("number.json");

	//
	m_pNumCounter->SetDrawInfo( m_numberInfo );
	m_pNumCounter->AddValue( ScoreRecorder::GetInstance()->GetScore() );

	return true;
}

void Result2D::Update()
{
	CallPadEvent();
}

void Result2D::DrawUpdate()
{
	// �w�i�`��
	if( m_textureResult.m_pTex2D ){
		m_textureResult.m_pTex2D->DrawUpdate2D();
	}
}