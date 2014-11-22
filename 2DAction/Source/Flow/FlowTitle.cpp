/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		�^�C�g����ʊǗ��N���X
 */
/* ====================================================================== */

#include "FlowTitle.h"
#include "System/Sound/SystemSoundManager.h"

FlowBase *FlowTitle::Create( const std::string &fileName )
{
	return NEW FlowTitle(fileName);
}

FlowTitle::FlowTitle( const std::string &fileName )
: FlowBase( fileName )
, m_pTitleTex( NULL )
{
	DEBUG_PRINT("FlowTitle�����I�I\n");
}


FlowTitle::~FlowTitle(void)
{
	DEBUG_PRINT("FlowTitle�폜�I�I\n");
}

bool FlowTitle::Finish()
{
	return true;
}

bool FlowTitle::Init()
{
	// �ꖇ�G�쐬
	m_pTitleTex = Title2D::CreateTitle2D();
	return true;
}

void FlowTitle::PadEventDecide()
{
	// ����SE�炷
	SoundManager::GetInstance()->PlaySE("Decide");
	StartFade("proceed");
}


/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		�^�C�g���ꖇ�G�N���X
 */
/* ====================================================================== */
Title2D *Title2D::CreateTitle2D()
{
	return NEW Title2D();
}

Title2D::Title2D()
: TaskUnit("Title2D")
, m_title2D( NULL )
{
	// �`��N���X�Z�b�g�A�b�v
	m_title2D = NEW Game2DBase("title.json");
	m_titleInfo.Init();
	m_titleInfo.m_pos.x = WINDOW_WIDTH / 2.0f;
	m_titleInfo.m_pos.y = WINDOW_HEIGHT / 2.0f;
	m_titleInfo.m_usePlayerOffset = false;
	m_title2D->SetDrawInfo(m_titleInfo);
}

Title2D::~Title2D(void)
{
	SAFE_DELETE( m_title2D );
}

void Title2D::DrawUpdate()
{
	if( m_title2D ){
		m_title2D->DrawUpdate2D();
	}
}