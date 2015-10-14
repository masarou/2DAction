/* ====================================================================== */
/**
 * @brief  背景管理クラス
 *
 * @note
 *		
 */
/* ====================================================================== */


#include "SystemBgManager.h"
#include "System/Draw2D/SystemDraw2DResource.h"
#include "Common/Utility/CommonGameUtility.h"

#include "Game/GameRecorder.h"

BgManager *BgManager::s_pInstance = NULL;

static struct BGMAP {
	std::string		name;
	BgManager::BGID	id;
}s_bgAttay[ BgManager::BGID_MAX ] =
{
	{	"BGID_TITLE",			BgManager::BGID_TITLE,			},
	{	"BGID_STAGE_RESULT",	BgManager::BGID_STAGE_RESULT,	},
	{	"BGID_COMMON",			BgManager::BGID_COMMON,	},
	{	"BGID_INTERVAL",		BgManager::BGID_INTERVAL,		},
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

/* ================================================ */
/**
 * @brief	次のBGセット
 */
/* ================================================ */
void BgManager::SetNextBg( const std::string &bgStr )
{
	for( uint32_t i = 0; i < NUMBEROF(s_bgAttay) ; ++i ){
		if( s_bgAttay[i].name.compare( bgStr ) == 0 ){
			SetNextBg( s_bgAttay[i].id );
			return;
		}
	}
	DEBUG_ASSERT( 0, "指定されたBGがない" );
}

void BgManager::SetNextBg( const BGID &bgId )
{
	if( bgId == BGID_CURRENT ){
		// 現状維持
		return;
	}

	// 次に設定されているBGがNULLでかつ、現在のBGと異なるときに新たにセット
	if( m_nextBGID == BGID_MAX && m_currBGID != bgId ){
		if( bgId < BGID_MAX ){
			m_nextBGID = bgId;
			SetNextStep( STEP_INIT );
		}
	}
	else if( m_nextBGID != BGID_MAX ){
		DEBUG_PRINT("SetNextBgが呼ばれたが既に予約済みだったので無視します\n");
	}
	else if( m_currBGID != bgId ){
		DEBUG_PRINT("SetNextBgが呼ばれたが既に設定しているBGだったので無視します\n");
	}
}

/* ================================================ */
/**
 * @brief	BGの表示準備が終了したかどうか
 */
/* ================================================ */
bool BgManager::IsShowingBG() const
{
	return ( m_currStep == STEP_SHOWING ) ? true : false ;
}

/* ================================================ */
/**
 * @brief	各種更新処理
 */
/* ================================================ */
void BgManager::Update()
{

	switch( m_currStep ){
	default:

		break;
	case STEP_INIT:
		{
			// 表示しているID更新
			m_currBGID = m_nextBGID;
			m_nextBGID = BGID_MAX;

			// 現在表示しているBG削除
			m_drawTexture.DeleteAndInit();

			// 次のステップへ
			SetNextStep( STEP_CHANGING );
		}
		break;
	case STEP_CHANGING:
		{
			if( m_currBGID != BGID_NONE ){
				// 描画クラスセットアップ
				std::string fileName = GetBgJsonName( m_currBGID );
				m_drawTexture.m_pTex2D = Game2DBase::Create( fileName.c_str() );
				TEX_DRAW_INFO drawInfo;
				drawInfo.Init();
				drawInfo.m_fileName = fileName;
				drawInfo.m_posOrigin.x = WINDOW_WIDTH / 2.0f;
				drawInfo.m_posOrigin.y = WINDOW_HEIGHT / 2.0f;
				drawInfo.m_usePlayerOffset = false;
				drawInfo.m_prioity = Common::PRIORITY_LOWEST;
				m_drawTexture.m_pTex2D->SetDrawInfo( drawInfo );
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

/* ================================================ */
/**
 * @brief	BGIDから読み込むべきjson名取得
 */
/* ================================================ */
std::string BgManager::GetBgJsonName( const BGID &bgId ) const
{
	std::string fileName = "";
	switch( bgId ){
	default:

		break;
	case BGID_TITLE:
		fileName = "TitleBg.json";
		break;
	case BGID_STAGE_RESULT:
		fileName = "TitleBg.json";
		break;
	case BGID_COMMON:
		fileName = "CommonBg.json";
		break;
	case BGID_INTERVAL:
		{
			GameRecorder *pRecorder = GameRecorder::GetInstance();
			switch( pRecorder->GetGameStateOfProgress() ){
			case GameRecorder::STATE_STAGE10:
			default:
				DEBUG_ASSERT( 0, "現在のゲームの状態が想定外");
				break;
			case GameRecorder::STATE_STAGE01:
			case GameRecorder::STATE_STAGE02:
				fileName = "ResultBgStage01.json";
				break;
			case GameRecorder::STATE_STAGE03:
			case GameRecorder::STATE_STAGE04:
			case GameRecorder::STATE_STAGE05:
				fileName = "ResultBgStage02.json";
				break;
			case GameRecorder::STATE_STAGE06:
			case GameRecorder::STATE_STAGE07:
			case GameRecorder::STATE_STAGE08:
			case GameRecorder::STATE_STAGE09:
				fileName = "ResultBgStage03.json";
				break;
			}
		}
		break;
	case BGID_NONE:
		break;
	}
	return fileName;
}



