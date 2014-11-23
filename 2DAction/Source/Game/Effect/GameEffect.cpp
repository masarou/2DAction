/* ====================================================================== */
/**
 * @brief  エフェクトクラス 基本一回再生+自殺
 *
 * @note
 *		
 */
/* ====================================================================== */

#include "GameEffect.h"


GameEffect::GameEffect( const EFFECT_KIND &kind, const uint32_t &posX, const uint32_t &posY )
	: TaskUnit("GameEffect")
	, m_kind( kind )
{
	// 描画クラスセットアップ
	m_textureEffect.Init();
	m_textureEffect.m_pTex2D = NEW Game2DBase(SelectEffectFile().c_str());
	m_textureEffect.m_texInfo.Init();
	m_textureEffect.m_texInfo.m_fileName = SelectEffectFile();
	m_textureEffect.m_texInfo.m_pos.x = static_cast<float>(posX);
	m_textureEffect.m_texInfo.m_pos.y = static_cast<float>(posY);
	m_textureEffect.m_pTex2D->SetAnim(SelectEffectAnimTag());
}

GameEffect::~GameEffect(void)
{
}

bool GameEffect::DieMain()
{
	m_textureEffect.DeleteAndInit();
	return true;
}

bool GameEffect::Init()
{
	m_textureEffect.m_pTex2D->SetDrawInfo( m_textureEffect.m_texInfo );
	return true;
}

void GameEffect::Update()
{
}

void GameEffect::DrawUpdate()
{
	if( std::string("").compare(m_textureEffect.m_pTex2D->GetPlayAnim()) == 0 ){
		// アニメは終わったので自殺
		TaskStartDie();
		return;
	}

	m_textureEffect.m_pTex2D->DrawUpdate2D();
}

std::string GameEffect::SelectEffectFile()
{
	std::string rtn = "";
	switch(m_kind){
	case EFFECT_BOMB:
		rtn = "effectBomb.json";
		break;

	default:
		DEBUG_ASSERT( 0,  "エフェクト種類が想定外" );
		// とりあえず一番無難なものをセット
		rtn = "effectBomb.json";
		break;
	}
	return rtn;
}

std::string GameEffect::SelectEffectAnimTag()
{
	std::string rtn = "";
	switch(m_kind){
	case EFFECT_BOMB:
		rtn = "bomb";
		break;

	default:
		DEBUG_ASSERT( 0,  "エフェクト種類が想定外" );
		rtn = "";
		break;
	}
	return rtn;
}
