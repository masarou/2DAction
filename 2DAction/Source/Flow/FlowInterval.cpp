/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		�Q�[���I���ナ�g���C�m�F��ʃN���X
 */
/* ====================================================================== */
#include "FlowInterval.h"
#include "Game/GameRecorder.h"
#include "System/Sound/SystemSoundManager.h"

FlowBase *FlowInterval::Create( const std::string &fileName )
{
	return NEW FlowInterval(fileName);
}

FlowInterval::FlowInterval( const std::string &fileName )
: FlowBase(fileName)
, m_pRetryTex( NULL )
{
	DEBUG_PRINT("FlowInterval�����I�I\n");
}


FlowInterval::~FlowInterval(void)
{
	DEBUG_PRINT("FlowInterval�폜�I�I\n");
}

bool FlowInterval::Init()
{
	// �ꖇ�G�쐬
	m_pRetryTex = Interval2D::CreateRetry2D();
	return true;
}

/* ================================================ */
/**
 * @brief	�p�b�h����֐�
 */
/* ================================================ */
void FlowInterval::PadEventDecide()
{
	// ����SE�炷
	SoundManager::GetInstance()->PlaySE("Decide");

	switch( m_pRetryTex->GetSelectedNo() ){
	default:
		break;
	case Interval2D::SELECT_NEXT:
		StartFade( GetNextFadeStr().c_str() );
		break;
	case Interval2D::SELECT_TITLE:
		StartFade("title");
		break;
	}
}

/* ================================================ */
/**
 * @brief	���ɐi�ނׂ����x���擾�֐�
 */
/* ================================================ */
std::string FlowInterval::GetNextFadeStr()
{
	std::string retStr = "";
	GameRecorder *pRecorder = GameRecorder::GetInstance();
	if( pRecorder ){
		switch( pRecorder->GetGameStateOfProgress() ){
		case GameRecorder::STATE_TITLE:
		case GameRecorder::STATE_STAGE03:
		default:
			DEBUG_ASSERT( 0, "�z��O�̃t���[" );
			// �Ƃ肠�����^�C�g����
			retStr = "title";
			break;
		case GameRecorder::STATE_STAGE01:
			retStr = "nextgame02";
			break;
		case GameRecorder::STATE_STAGE02:
			retStr = "nextgame03";
			break;
		}
	}
	return retStr;
}


/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		�^�C�g���ꖇ�G�N���X
 */
/* ====================================================================== */
Interval2D *Interval2D::CreateRetry2D()
{
	return NEW Interval2D();
}

Interval2D::Interval2D()
: TaskUnit("Interval2D")
, m_selectNo( 0 )
{
	m_textureRetry.Init();

	// �`��N���X�Z�b�g�A�b�v
	m_textureRetry.m_pTex2D = NEW Game2DBase("title.json");
	m_textureRetry.m_texInfo.Init();
	m_textureRetry.m_texInfo.m_posOrigin.x = WINDOW_WIDTH / 2.0f;
	m_textureRetry.m_texInfo.m_posOrigin.y = WINDOW_HEIGHT / 2.0f;
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

Interval2D::~Interval2D(void)
{
	m_textureRetry.DeleteAndInit();
	
	for( uint32_t i = 0; i < SELECT_RETRY_MAX; ++i ){
		SAFE_DELETE( m_pTexChoiceArray[i] );
		SAFE_DELETE( m_pTexChoiceBGArray[i] );
	}
}


bool Interval2D::Init()
{
	// �����L�[�̃p�b�h�C�x���g��PUSH�ŌĂԂ悤�ɐݒ�
	SetPadButtonState( InputWatcher::BUTTON_UP,		InputWatcher::EVENT_PUSH );
	SetPadButtonState( InputWatcher::BUTTON_DOWN,	InputWatcher::EVENT_PUSH );
	SetPadButtonState( InputWatcher::BUTTON_RIGHT,	InputWatcher::EVENT_PUSH );
	SetPadButtonState( InputWatcher::BUTTON_LEFT,	InputWatcher::EVENT_PUSH );

	return true;
}

void Interval2D::PadEventUp()
{
}

void Interval2D::PadEventDown()
{
}
void Interval2D::PadEventRight()
{
	// �J�[�\��SE�炷
	SoundManager::GetInstance()->PlaySE("Cursor");
	m_selectNo = (m_selectNo+1) % SELECT_RETRY_MAX;
}
void Interval2D::PadEventLeft()
{
	// �J�[�\��SE�炷
	SoundManager::GetInstance()->PlaySE("Cursor");
	m_selectNo = (m_selectNo+(SELECT_RETRY_MAX - 1)) % SELECT_RETRY_MAX;
}

void Interval2D::Update()
{
	CallPadEvent();
}

void Interval2D::DrawUpdate()
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
			m_texInfo.m_posOrigin = math::Vector2( 100.0f, 40.0f );
			break;
		case SELECT_NEXT:
			m_pTexChoiceArray[i]->SetAnim("retry");
			m_texInfo.m_posOrigin = math::Vector2( 100.0f, 40.0f );
			break;
		case SELECT_TITLE:
			m_pTexChoiceArray[i]->SetAnim("title");
			m_texInfo.m_posOrigin = math::Vector2( 400.0f, 40.0f );
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
