/* ====================================================================== */
/**
 * @brief  �w�i�Ǘ��N���X
 *
 * @note
 *		
 */
/* ====================================================================== */


#include "SystemBgManager.h"
#include "System/Draw2D/SystemDraw2DResource.h"
#include "Common/Utility/CommonGameUtility.h"

BgManager *BgManager::s_pInstance = NULL;

static struct BGMAP {
	std::string		name;
	BgManager::BGID	id;
}s_bgAttay[ BgManager::BGID_MAX ] =
{
	{	"BGID_TITLE",			BgManager::BGID_TITLE,			},
	{	"BGID_STAGE_RESULT",	BgManager::BGID_STAGE_RESULT,	},
	{	"BGID_COMMON",			BgManager::BGID_COMMON,	},
	{	"BGID_CURRENT",			BgManager::BGID_CURRENT,		},
	{	"BGID_NONE",			BgManager::BGID_NONE,			},
};

BgManager *BgManager::Create()
{
	if( !s_pInstance ){
		s_pInstance = NEW BgManager();
	}
	return s_pInstance;
}

BgManager *BgManager::GetInstance()
{
	return s_pInstance;
}


BgManager::BgManager()
: TaskUnit("BgManager")
, m_currStep( STEP_MAX )
, m_currBGID( BGID_MAX )
, m_nextBGID( BGID_MAX )
{
	m_drawTexture.Init();
}


BgManager::~BgManager(void)
{
	s_pInstance = NULL;
}

bool BgManager::DieMain()
{
	m_drawTexture.DeleteAndInit();
	return true;
}

void BgManager::SetNextBg( const std::string &bgStr )
{
	for( uint32_t i = 0; i < NUMBEROF(s_bgAttay) ; ++i ){
		if( s_bgAttay[i].name.compare( bgStr ) == 0 ){
			SetNextBg( s_bgAttay[i].id );
			return;
		}
	}
	DEBUG_ASSERT( 0, "�w�肳�ꂽBG���Ȃ�" );
}

void BgManager::SetNextBg( const BGID &bgId )
{
	if( bgId == BGID_CURRENT ){
		// ����ێ�
		return;
	}

	// ���ɐݒ肳��Ă���BG��NULL�ł��A���݂�BG�ƈقȂ�Ƃ��ɐV���ɃZ�b�g
	if( m_nextBGID == BGID_MAX && m_currBGID != bgId ){
		if( bgId < BGID_MAX ){
			m_nextBGID = bgId;
			SetNextStep( STEP_INIT );
		}
	}
	else if( m_nextBGID != BGID_MAX ){
		DEBUG_PRINT("SetNextBg���Ă΂ꂽ�����ɗ\��ς݂������̂Ŗ������܂�\n");
	}
	else if( m_currBGID != bgId ){
		DEBUG_PRINT("SetNextBg���Ă΂ꂽ�����ɐݒ肵�Ă���BG�������̂Ŗ������܂�\n");
	}
}

bool BgManager::IsShowingBG() const
{
	return ( m_currStep == STEP_SHOWING ) ? true : false ;
}

void BgManager::Update()
{

	switch( m_currStep ){
	default:

		break;
	case STEP_INIT:
		{
			// �\�����Ă���ID�X�V
			m_currBGID = m_nextBGID;
			m_nextBGID = BGID_MAX;

			// ���ݕ\�����Ă���BG�폜
			m_drawTexture.DeleteAndInit();

			// ���̃X�e�b�v��
			SetNextStep( STEP_CHANGING );
		}
		break;
	case STEP_CHANGING:
		{
			if( m_currBGID != BGID_NONE ){
				// �`��N���X�Z�b�g�A�b�v
				std::string fileName = GetBgJsonName( m_currBGID );
				m_drawTexture.m_pTex2D = NEW Game2DBase( fileName.c_str() );
				m_drawTexture.m_texInfo.m_fileName = fileName;
				m_drawTexture.m_texInfo.m_posOrigin.x = WINDOW_WIDTH / 2.0f;
				m_drawTexture.m_texInfo.m_posOrigin.y = WINDOW_HEIGHT / 2.0f;
				m_drawTexture.m_texInfo.m_usePlayerOffset = false;
				m_drawTexture.m_texInfo.m_prioity = PRIORITY_LOWEST;
				m_drawTexture.m_pTex2D->SetDrawInfo(m_drawTexture.m_texInfo);
			}

			SetNextStep( STEP_WAIT );
		}
		break;
	case STEP_WAIT:
		SetNextStep( STEP_SHOWING );
		break;
	case STEP_SHOWING:
		break;

	}
}

void BgManager::DrawUpdate()
{
	if( m_currStep == STEP_SHOWING && m_drawTexture.m_pTex2D ){
		m_drawTexture.m_pTex2D->DrawUpdate2D();
	}
}

std::string BgManager::GetBgJsonName( const BGID &bgId ) const
{
	std::string fileName = "";
	switch( bgId ){
	default:

		break;
	case BGID_TITLE:
		fileName = "titleBg.json";
		break;
	case BGID_STAGE_RESULT:
		fileName = "titleBg.json";
		break;
	case BGID_COMMON:
		fileName = "commonBg.json";
		break;
	case BGID_CURRENT:
	case BGID_NONE:
		break;
	}
	return fileName;
}



