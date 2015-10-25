/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		操作説明演出クラス
 */
/* ====================================================================== */

#include "FlowProcessStageStart.h"
#include "Flow/FlowManager.h"
#include "Game/GameRegister.h"
#include "System/Draw2D/SystemDraw2DResource.h"


StageStart *StageStart::Create()
{
	return NEW StageStart();
}

StageStart::StageStart(void)
: m_counter( 0 )
, m_texSizeW( 0 )
, m_IsFadeOut( false )
{
	m_stagePhaseTex.Init();
	m_stageClearTex.Init();
}


StageStart::~StageStart(void)
{
}

bool StageStart::CanDie()
{
	m_stagePhaseTex.DeleteAndInit();
	m_stageClearTex.DeleteAndInit();
	return true;
}

bool StageStart::Init()
{
	m_stagePhaseTex.m_pTex2D = Game2DBase::Create("StageStart01.json");
	m_stageClearTex.m_pTex2D = Game2DBase::Create("StageStart02.json");
	
	// 画像サイズを保持して遷移の指標に使う
	const TEX_INIT_INFO &texInfo = TextureResourceManager::GetInstance()->GetLoadTextureInfo( "StageStart01.json" );
	m_texSizeW = texInfo.m_sizeWidth;

	// ステージフェーズ画像
	TEX_DRAW_INFO drawInfoPhase;
	drawInfoPhase.Init();
	drawInfoPhase.m_fileName = "StageStart01.json";
	drawInfoPhase.m_posOrigin = math::Vector2( static_cast<float>( -1.0f*m_texSizeW ), 200.0f );
	drawInfoPhase.m_usePlayerOffset = false;
	drawInfoPhase.m_prioity = Common::PRIORITY_HIGHEST;
	m_stagePhaseTex.m_pTex2D->SetDrawInfo( drawInfoPhase );
	m_stagePhaseTex.m_pTex2D->SetAnim( GetStageAnimStr().c_str() );

	// クリア条件画像
	TEX_DRAW_INFO drawInfoClear;
	drawInfoClear.Init();
	drawInfoClear.m_fileName = "StageStart02.json";
	drawInfoClear.m_posOrigin = math::Vector2( static_cast<float>( -1.0f*m_texSizeW ), 520.0f );
	drawInfoClear.m_usePlayerOffset = false;
	drawInfoClear.m_prioity = Common::PRIORITY_HIGHEST;
	m_stageClearTex.m_pTex2D->SetDrawInfo( drawInfoClear );
	m_stageClearTex.m_pTex2D->SetAnim( GetClearAnimStr().c_str() );

	return true;
}


void StageStart::Update()
{
	if( !m_stagePhaseTex.m_pTex2D ){
		DEBUG_ASSERT( 0, "m_stagePhaseTex.m_pTex2D is NULL");
		return;
	}
	
	TEX_DRAW_INFO &info01 = m_stagePhaseTex.m_pTex2D->UpdateDrawInfo();
	TEX_DRAW_INFO &info02 = m_stageClearTex.m_pTex2D->UpdateDrawInfo();

	// 移動更新
	FadeUpdate( info01 );
	if( m_counter > 30 ){
		FadeUpdate( info02 );
	}

	// 描画
	if( m_stagePhaseTex.m_pTex2D ){
		m_stagePhaseTex.m_pTex2D->DrawUpdate2D();
	}
	if( m_stageClearTex.m_pTex2D ){
		m_stageClearTex.m_pTex2D->DrawUpdate2D();
	}

	// 演出終了チェック
	if( info02.m_posOrigin.x > m_texSizeW*2.0f ){
		SetStateNext();
	}

	++m_counter;
}

void StageStart::PadEventDecide()
{
	m_IsFadeOut = true;
}

void StageStart::FadeUpdate( TEX_DRAW_INFO &info )
{
	uint32_t diff = math::Abs( static_cast<uint32_t>( info.m_posOrigin.x - (WINDOW_WIDTH/2) ) );
	diff *= 0.1f;
	if( diff <= 0 && m_IsFadeOut ){
		diff = 1;
	}
	info.m_posOrigin.x += diff;
}


std::string StageStart::GetStageAnimStr()
{
	Common::GAME_FLOW flowKind = FlowManager::GetInstance()->GetCurrentFlowKind();
	std::string retStr = "";

	switch( flowKind ){
	default:
		DEBUG_ASSERT( 0, "現在のゲームの状態が想定外");
		break;
	case Common::FLOW_STAGE01:
		retStr = "1-1";
		break;
	case Common::FLOW_STAGE02:
		retStr = "1-2";
		break;
	case Common::FLOW_STAGE03:
		retStr = "1-3";
		break;
	case Common::FLOW_STAGE04:
		retStr = "1-4";
		break;
	case Common::FLOW_STAGE05:
		retStr = "2-1";
		break;
	case Common::FLOW_STAGE06:
		retStr = "2-2";
		break;
	case Common::FLOW_STAGE07:
		retStr = "2-3";
		break;
	case Common::FLOW_STAGE08:
		retStr = "3-1";
		break;
	case Common::FLOW_STAGE09:
		retStr = "3-2";
		break;
	case Common::FLOW_STAGE10:
		retStr = "3-3";
		break;
	}
	return retStr;
}


std::string StageStart::GetClearAnimStr()
{
	const GameManager *pGameManager = GameRegister::GetInstance()->GetManagerGame();
	std::string retStr = "";

	switch( pGameManager->GetStageType() ){
	default:
		DEBUG_ASSERT( 0, "ゲームのクリア条件が想定外");
		break;
	case GameManager::TYPE_TIME:
		retStr = "time";
		break;
	case GameManager::TYPE_DESTROY:
		retStr = "destroy";
		break;
	case GameManager::TYPE_DESTROY_BOSS:
		retStr = "boss";
		break;
	}
	return retStr;
}

