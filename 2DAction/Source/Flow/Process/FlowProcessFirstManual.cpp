/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		操作説明演出クラス
 */
/* ====================================================================== */

#include "FlowProcessFirstManual.h"
#include "System/Sound/SystemSoundManager.h"
#include "System/Draw2D/SystemDraw2DResource.h"

FirstManual *FirstManual::Create( const MANUAL_KIND &kind )
{
	return NEW FirstManual( kind );
}

FirstManual::FirstManual( const MANUAL_KIND &kind )
: m_kind( kind )
, m_step( STEP_MAX )
{
}


FirstManual::~FirstManual(void)
{
}

bool FirstManual::CanDie()
{
	m_controllTex.DeleteAndInit();
	return true;
}

bool FirstManual::Init()
{
	TEX_DRAW_INFO drawInfo;
	m_controllTex.Init();
	m_controllTex.m_pTex2D = Game2DBase::Create( GetFilePath().c_str() );

	const TEX_INIT_INFO &texInfo	= TextureResourceManager::GetInstance()->GetLoadTextureInfo( GetFilePath().c_str() );
	drawInfo.m_prioity			= PRIORITY_HIGH;
	drawInfo.m_posOrigin		= math::Vector2( 0.0f, texInfo.m_sizeHeight / 2.0f );
	drawInfo.m_arrangeOrigin	= math::Vector2( 0.0f, texInfo.m_sizeHeight / 2.0f );
	drawInfo.m_usePlayerOffset	= false;
	drawInfo.m_rot				= -180.0f;
	m_controllTex.m_pTex2D->SetDrawInfo( drawInfo );

	m_step = STEP_IN;
	return true;
}


void FirstManual::Update()
{
	Game2DBase *pTex2D = m_controllTex.m_pTex2D;
	if( pTex2D ){
		TEX_DRAW_INFO &drawInfo = pTex2D->UpdateDrawInfo();
		float rot = drawInfo.m_rot.GetDegree();
		switch( m_step ){
		case STEP_IN:
			rot *= 1.02f;
			drawInfo.m_rot = math::Angle( rot );
			if( rot > 360.0f ){
				ChangeAnimeStepNext();
			}
			break;

		case STEP_WAIT:
			break;

		case STEP_OUT:
			rot *= 1.15f;
			drawInfo.m_rot = math::Angle( rot );
			if( rot > 180.0f ){
				ChangeAnimeStepNext();
			}
			break;
		default:
			break;
		};
		pTex2D->DrawUpdate2D();
	}
}

void FirstManual::PadEventDecide()
{
	SoundManager::GetInstance()->PlaySE("Decide");
	ChangeAnimeStepNext();
}

const std::string FirstManual::GetFilePath() const
{
	std::string retStr = "";
	switch( m_kind ){
	default:
		DEBUG_PRINT( "初回説明画像のファイルパスが想定外" );
	case KIND_POWERUP:
		retStr = "FirstManualPowerUp.json";
		break;
	}

	return retStr;
}

void FirstManual::ChangeAnimeStepNext()
{
	switch( m_step ){
	case STEP_IN:
		{
			Game2DBase *pTex2D = m_controllTex.m_pTex2D;
			if( pTex2D ){
				TEX_DRAW_INFO &drawInfo = pTex2D->UpdateDrawInfo();
				drawInfo.m_rot = math::Angle( 1.0f );
			}
			m_step = STEP_WAIT;
		}
		break;

	case STEP_WAIT:
		m_step = STEP_OUT;
		break;

	case STEP_OUT:
		{
			Game2DBase *pTex2D = m_controllTex.m_pTex2D;
			if( pTex2D ){
				TEX_DRAW_INFO &drawInfo = pTex2D->UpdateDrawInfo();
				drawInfo.m_rot = math::Angle( 180.0f );
			}
			// クラス終了
			SetStateNext();
		}
		m_step = STEP_END;
		break;
	default:
		break;
	};
}
