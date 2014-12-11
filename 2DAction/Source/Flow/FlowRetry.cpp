/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		�Q�[���I���ナ�g���C�m�F��ʃN���X
 */
/* ====================================================================== */
#include "FlowRetry.h"
#include "Game/GameScoreRecorder.h"
#include "System/Sound/SystemSoundManager.h"

FlowBase *FlowRetry::Create( const std::string &fileName )
{
	return NEW FlowRetry(fileName);
}

FlowRetry::FlowRetry( const std::string &fileName )
: FlowBase(fileName)
, m_pRetryTex( NULL )
{
	DEBUG_PRINT("FlowRetry�����I�I\n");
}


FlowRetry::~FlowRetry(void)
{
	DEBUG_PRINT("FlowRetry�폜�I�I\n");
}

bool FlowRetry::Init()
{
	// �ꖇ�G�쐬
	m_pRetryTex = Retry2D::CreateRetry2D();
	return true;
}

bool FlowRetry::Finish()
{
	// �풓���ŏ��������K�v�Ȃ��̃��Z�b�g

	// �X�R�A������
	ScoreRecorder::GetInstance()->ScoreReset();
	// �v���C���[�I�t�Z�b�g���Z�b�g
	GameAccesser::GetInstance()->InitAll();

	return true;
}

/* ================================================ */
/**
 * @brief	�p�b�h����֐�
 */
/* ================================================ */
void FlowRetry::PadEventDecide()
{
	// ����SE�炷
	SoundManager::GetInstance()->PlaySE("Decide");

	switch( m_pRetryTex->GetSelectedNo() ){
	default:
		
		break;
	case Retry2D::SELECT_RETRY:
	StartFade("retrygame");
		break;
	case Retry2D::SELECT_TITLE:
	StartFade("title");
		break;
	}
}


/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		�^�C�g���ꖇ�G�N���X
 */
/* ====================================================================== */
Retry2D *Retry2D::CreateRetry2D()
{
	return NEW Retry2D();
}

Retry2D::Retry2D()
: TaskUnit("Retry2D")
, m_selectNo( 0 )
{
	m_textureRetry.Init();

	// �`��N���X�Z�b�g�A�b�v
	m_textureRetry.m_pTex2D = NEW Game2DBase("title.json");
	m_textureRetry.m_texInfo.Init();
	m_textureRetry.m_texInfo.m_pos.x = WINDOW_WIDTH / 2.0f;
	m_textureRetry.m_texInfo.m_pos.y = WINDOW_HEIGHT / 2.0f;
	m_textureRetry.m_texInfo.m_usePlayerOffset = false;
	m_textureRetry.m_pTex2D->SetDrawInfo(m_textureRetry.m_texInfo);

	m_texInfo.Init();
	m_texInfo.m_usePlayerOffset = false;
	for( uint32_t i = 0; i < SELECT_RETRY_MAX; ++i ){
		m_pTexChoiceArray[i] = NULL;
		m_pTexChoiceBGArray[i] = NULL;

		m_pTexChoiceArray[i] = NEW Game2DBase("choice.json");
		m_pTexChoiceArray[i]->SetDrawInfo( m_texInfo );
		m_pTexChoiceBGArray[i] = NEW Game2DBase("choiceBG.json");
		m_pTexChoiceBGArray[i]->SetDrawInfo( m_texInfo );
	}
}

Retry2D::~Retry2D(void)
{
	m_textureRetry.DeleteAndInit();
	
	for( uint32_t i = 0; i < SELECT_RETRY_MAX; ++i ){
		SAFE_DELETE( m_pTexChoiceArray[i] );
		SAFE_DELETE( m_pTexChoiceBGArray[i] );
	}
}


bool Retry2D::Init()
{
	// �����L�[�̃p�b�h�C�x���g��PUSH�ŌĂԂ悤�ɐݒ�
	SetPadButtonState( InputWatcher::BUTTON_UP,		InputWatcher::EVENT_PUSH );
	SetPadButtonState( InputWatcher::BUTTON_DOWN,	InputWatcher::EVENT_PUSH );
	SetPadButtonState( InputWatcher::BUTTON_RIGHT,	InputWatcher::EVENT_PUSH );
	SetPadButtonState( InputWatcher::BUTTON_LEFT,	InputWatcher::EVENT_PUSH );

	return true;
}

void Retry2D::PadEventUp()
{
}

void Retry2D::PadEventDown()
{
}
void Retry2D::PadEventRight()
{
	// �J�[�\��SE�炷
	SoundManager::GetInstance()->PlaySE("Cursor");
	m_selectNo = (m_selectNo+1) % SELECT_RETRY_MAX;
}
void Retry2D::PadEventLeft()
{
	// �J�[�\��SE�炷
	SoundManager::GetInstance()->PlaySE("Cursor");
	m_selectNo = (m_selectNo+(SELECT_RETRY_MAX - 1)) % SELECT_RETRY_MAX;
}

void Retry2D::Update()
{
	CallPadEvent();
}

void Retry2D::DrawUpdate()
{
	if( m_textureRetry.m_pTex2D ){
		m_textureRetry.m_pTex2D->DrawUpdate2D();
	}
	
	for( uint32_t i = 0; i < SELECT_RETRY_MAX; ++i){
		if( !m_pTexChoiceArray[i] || !m_pTexChoiceBGArray[i] ){
			continue;
		}
		switch(i){
		default:
			DEBUG_ASSERT( 0, "�z��O�̒l" );
			m_pTexChoiceArray[i]->SetAnim("title");
			m_texInfo.m_pos = math::Vector2( 100.0f, 40.0f );
			break;
		case SELECT_RETRY:
			m_pTexChoiceArray[i]->SetAnim("retry");
			m_texInfo.m_pos = math::Vector2( 100.0f, 40.0f );
			break;
		case SELECT_TITLE:
			m_pTexChoiceArray[i]->SetAnim("title");
			m_texInfo.m_pos = math::Vector2( 400.0f, 40.0f );
			break;
		}

		// �J�[�\�����������Ă�����A�j���ύX
		std::string anim = "default";
		if( m_selectNo == i ){
			anim = "spot";
		}
		m_pTexChoiceBGArray[i]->SetAnim( anim );

		// �`��ʒu�X�V
		m_pTexChoiceArray[i]->SetDrawInfo( m_texInfo );
		m_pTexChoiceBGArray[i]->SetDrawInfo( m_texInfo );

		// �`��
		m_pTexChoiceBGArray[i]->DrawUpdate2D();
		m_pTexChoiceArray[i]->DrawUpdate2D();
	}
}
