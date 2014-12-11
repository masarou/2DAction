/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		�Q�[������\���N���X
 */
/* ====================================================================== */

#include "FlowViewScore.h"
#include "Common/Utility/CommonGameUtility.h"
#include "System/Sound/SystemSoundManager.h"

FlowBase *FlowViewScore::Create( const std::string &fileName )
{
	return NEW FlowViewScore(fileName);
}

FlowViewScore::FlowViewScore( const std::string &fileName )
: FlowBase(fileName)
{
	DEBUG_PRINT("FlowViewScore�����I�I\n");
}


FlowViewScore::~FlowViewScore(void)
{
	DEBUG_PRINT("FlowResult�폜�I�I\n");
}

bool FlowViewScore::Init()
{
	// �w�i�ꖇ�G�쐬
	m_pViewScoreTex = ViewScore2D::CreateViewScore2D();
	return true;
}

/* ================================================ */
/**
 * @brief	�p�b�h����֐�
 */
/* ================================================ */
void FlowViewScore::PadEventCancel()
{
	// �L�����Z��SE�炷
	SoundManager::GetInstance()->PlaySE("Cancel");
	StartFade("return");
}


/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		�^�C�g���ꖇ�G�N���X
 */
/* ====================================================================== */
ViewScore2D *ViewScore2D::CreateViewScore2D()
{
	return NEW ViewScore2D();
}

ViewScore2D::ViewScore2D()
: TaskUnit("ViewScore2D")
{
	// �����\���p�摜���
	m_numberInfo.Init();
	m_numberInfo.m_pos.x = WINDOW_WIDTH - 300.0f;
	m_numberInfo.m_pos.y = 100.0f;
	m_numberInfo.m_usePlayerOffset = false;
}

ViewScore2D::~ViewScore2D(void)
{
	m_textureResult.DeleteAndInit();
}

bool ViewScore2D::Init()
{
	// �w�i�쐬
	m_textureResult.Init();
	m_textureResult.m_pTex2D = NEW Game2DBase("title.json");
	m_textureResult.m_texInfo.Init();
	m_textureResult.m_texInfo.m_pos.x = WINDOW_WIDTH / 2.0f;
	m_textureResult.m_texInfo.m_pos.y = WINDOW_HEIGHT / 2.0f;
	m_textureResult.m_texInfo.m_usePlayerOffset = false;
	m_textureResult.m_pTex2D->SetDrawInfo(m_textureResult.m_texInfo);

	// �����L���O�擾
	GetSaveRanking( m_scoreData );

	// �����L���O�`��
	for( uint32_t i = 0; i < Common::RANKING_RECORD_MAX; ++i ){
		m_pNumCounter[i] = NumberCounter::Create("number.json");
		m_numberInfo.m_pos.y += 100.0f;
		m_numberInfo.m_scale = math::Vector2(2.0f,2.0f);
		m_pNumCounter[i]->SetDrawInfo( m_numberInfo );
		m_pNumCounter[i]->AddValue( m_scoreData.m_scoreTimeAttack[i] );
	}

	return true;
}

void ViewScore2D::Update()
{
	CallPadEvent();
}

void ViewScore2D::DrawUpdate()
{
	// �w�i�`��
	if( m_textureResult.m_pTex2D ){
		m_textureResult.m_pTex2D->DrawUpdate2D();
	}
}