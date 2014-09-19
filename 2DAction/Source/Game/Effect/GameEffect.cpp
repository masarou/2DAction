/* ====================================================================== */
/**
 * @brief  エフェクトクラス 基本一回再生+自殺
 *
 * @note
 *		
 */
/* ====================================================================== */

#include "GameEffect.h"


GameEffect::GameEffect( EFFECT_KIND kind, uint32_t posX, uint32_t posY )
	: TaskUnit("GameEffect")
	, m_kind( kind )
	, m_pEffect( NULL )
{
	// 描画クラスセットアップ
	m_pEffect = NEW Game2DBase(SelectEffectFile().c_str());
	m_effectInfo.Init();
	m_effectInfo.m_pos.x = posX;
	m_effectInfo.m_pos.y = posY;
	m_pEffect->SetDrawInfo(m_effectInfo);
	m_pEffect->SetAnim(SelectEffectAnimTag());
}

GameEffect::~GameEffect(void)
{
}

bool GameEffect::DieMain()
{
	SAFE_DELETE(m_pEffect);
	return true;
}

void GameEffect::Update()
{


}

void GameEffect::DrawUpdate()
{
	if( std::string("").compare(m_pEffect->GetPlayAnim()) == 0 ){
		// アニメは終わったので自殺
		TaskStartDie();
		return;
	}

	m_pEffect->DrawUpdate2D();
}

std::string GameEffect::SelectEffectFile()
{
	std::string rtn = "";
	switch(m_kind){
	case EFFECT_BOMB:
		rtn = "effectBomb.json";
		break;

	default:
		DEBUG_ASSERT( "エフェクト種類が想定外" );
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
		DEBUG_ASSERT( "エフェクト種類が想定外" );
		rtn = "";
		break;
	}
	return rtn;
}
