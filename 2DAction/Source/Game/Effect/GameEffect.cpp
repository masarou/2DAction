/* ====================================================================== */
/**
 * @brief  エフェクトクラス 基本一回再生+自殺
 *
 * @note
 *		
 */
/* ====================================================================== */

#include "GameEffect.h"
#include "System/Draw2D/SystemDraw2DResource.h"
#include "Common/Utility/CommonGameUtility.h"

GameEffect::GameEffect( const EFFECT_KIND &kind, const int32_t &posX, const int32_t &posY )
	: TaskUnit("GameEffect")
	, m_kind( kind )
{
	// 描画クラスセットアップ
	m_textureEffect.Init();
	m_textureEffect.m_pTex2D = NEW Game2DBase( SelectEffectFile().c_str() );
	m_textureEffect.m_texInfo.m_fileName = SelectEffectFile();
	m_textureEffect.m_texInfo.m_posOrigin.x = static_cast<float>(posX);
	m_textureEffect.m_texInfo.m_posOrigin.y = static_cast<float>(posY);
	m_textureEffect.m_texInfo.m_prioity = PRIORITY_ABOVE_NORMAL;
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




/* ====================================================================== */
/**
 * @brief  ダメージを食らったとき専用エフェクトクラス 基本一回再生+自殺
 *
 * @note
 *		
 */
/* ====================================================================== */
GameEffectDamage *GameEffectDamage::s_pInstance = NULL;

GameEffectDamage *GameEffectDamage::GetInstance()
{
	if( !s_pInstance ){
		s_pInstance = NEW GameEffectDamage();
	}
	return s_pInstance;
}

GameEffectDamage::GameEffectDamage()
	: TaskUnit("GameEffectDamage")
{

}

GameEffectDamage::~GameEffectDamage()
{
	s_pInstance = NULL;
}

bool GameEffectDamage::Init()
{
	return true;
}

bool GameEffectDamage::DieMain()
{
	auto it = m_damageArray.begin();
	for(; it != m_damageArray.end() ;){
		EFFECT_DAMAGE_INFO info = (*it);
		it = m_damageArray.erase(it);
		for( uint32_t j = 0; j < info.m_array2D.size(); ++j){
			info.m_array2D.at(j).DeleteAndInit();
		}
	}
	return true;
}

void GameEffectDamage::CreateEffectDamage( const uint32_t &value, const int32_t &posX, const int32_t &posY )
{
	EFFECT_DAMAGE_INFO damageInfo;
	damageInfo.Init();
	damageInfo.m_value = value;

	// 表示する数字の桁数を求める
	uint32_t digitNum = 0;
	uint32_t rest = value;
	for(;rest != 0;++digitNum){
		rest /= 10;
	}

	// 桁数分push
	math::Vector2 basePos = math::Vector2( static_cast<float>(posX), static_cast<float>(posY) );
	// いい感じに位置調整
	basePos.x += (digitNum/2) * 15.0f + 8.0f;
	basePos.x += Utility::GetRandamValueFloat( 20, -20 );
	basePos.y += Utility::GetRandamValueFloat( 20, -20 );

	for(;damageInfo.m_array2D.size() < digitNum;){
		Texture2D tex;
		tex.Init();
		tex.m_pTex2D = NEW Game2DBase("damageNum.json");
		const TEX_INIT_INFO &texInfo = TextureResourceManager::GetInstance()->GetLoadTextureInfo("damageNum.json");
		basePos.x -= texInfo.m_sizeWidth;
		tex.m_texInfo.m_posOrigin		= basePos;
		tex.m_texInfo.m_prioity	= PRIORITY_HIGH;
		tex.m_pTex2D->SetDrawInfo( tex.m_texInfo );
		damageInfo.m_array2D.push_back( tex );
	}

	// 各桁で表示するアニメセット
	rest = value;
	for( uint32_t i = 0; rest != 0 ; ++i ){
		uint8_t underDigit = rest%10;
		std::string anim;
		anim += '0' + underDigit;
		damageInfo.m_array2D.at(i).m_pTex2D->SetAnim( anim.c_str() );

		rest /= 10;
	}

	if( damageInfo.m_array2D.size() == 0 ){
		DEBUG_ASSERT( 0, "aaa");
	}

	m_damageArray.push_back( damageInfo );
}

void GameEffectDamage::Update()
{
	for( uint32_t i = 0; i < m_damageArray.size(); ++i ){
		++m_damageArray.at(i).m_liveTime;
	}
}

void GameEffectDamage::DrawUpdate()
{
	// アニメが終わっているものがあるかどうか確認
	auto it = m_damageArray.begin();
	for( uint32_t i = 0; i < m_damageArray.size() ; ++i ){
		if( std::string("").compare(m_damageArray.at(i).m_array2D.at(0).m_pTex2D->GetPlayAnim()) == 0 ){
			EFFECT_DAMAGE_INFO info = (*it);
			it = m_damageArray.erase(it);
			for( uint32_t j = 0; j < info.m_array2D.size(); ++j){
				info.m_array2D.at(j).DeleteAndInit();
			}
		}
		else{
			++it;
		}
	}

	// 描画
	for( uint32_t i = 0; i < m_damageArray.size() ; ++i ){
		for( uint32_t j = 0; j < m_damageArray.at(i).m_array2D.size(); ++j){
			m_damageArray.at(i).m_array2D.at(j).m_texInfo.m_posOrigin.y -= static_cast<float>( 2 );
			m_damageArray.at(i).m_array2D.at(j).m_pTex2D->SetDrawInfo( m_damageArray.at(i).m_array2D.at(j).m_texInfo );
			m_damageArray.at(i).m_array2D.at(j).m_pTex2D->DrawUpdate2D();
		}
	}
}