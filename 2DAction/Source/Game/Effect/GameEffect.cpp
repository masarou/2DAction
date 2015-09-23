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
#include "System/Sound/SystemSoundManager.h"
#include "Common/Utility/CommonGameUtility.h"

class GameEffectWithCollision;

GameEffect *GameEffect::CreateEffect( const EFFECT_KIND &kind, const math::Vector2 &pos )
{
	return NEW GameEffect( kind, pos );
}

GameEffect *GameEffect::CreateEffect( const EFFECT_KIND &kind, const int32_t &posX, const int32_t &posY )
{
	return NEW GameEffect( kind, math::Vector2( static_cast<float>(posX), static_cast<float>(posY) ) );
}

GameEffect::GameEffect( const EFFECT_KIND &kind, const math::Vector2 &pos )
	: TaskUnit("GameEffect")
	, m_kind( kind )
{
	std::string readFileStr = SelectEffectFile();

	// 描画クラスセットアップ
	TEX_DRAW_INFO drawInfo;
	m_textureEffect.Init();
	m_textureEffect.m_pTex2D = Game2DBase::Create( readFileStr.c_str() );
	drawInfo.m_fileName = readFileStr;
	drawInfo.m_posOrigin = pos;
	drawInfo.m_prioity = Common::PRIORITY_ABOVE_NORMAL;
	if( m_kind == EFFECT_SLASHING_HIT ){
		// 斬撃HITはランダムに回転
		uint32_t rotate = Utility::GetRandamValue( 360, 0 );
		drawInfo.m_rot = static_cast<float>(rotate);
	}
	if( m_kind == EFFECT_DASH_SMOKE ){
		drawInfo.m_prioity = Common::PRIORITY_BELOW_NORMAL;
	}
	m_textureEffect.m_pTex2D->SetDrawInfo( drawInfo );
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

		if( m_kind == EFFECT_PRE_EXPLOSION ){
			GameEffectWithCollision::CreateEffect( Common::OWNER_ENEMY, GameEffectWithCollision::EFFECT_EXPLOSION, m_textureEffect.m_pTex2D->GetDrawInfo().m_posOrigin );
		}
		return;
	}

	m_textureEffect.m_pTex2D->DrawUpdate2D();
}

std::string GameEffect::SelectEffectFile() const
{
	std::string rtn = "";
	switch(m_kind){
	case EFFECT_BOMB:
		rtn = "EffectBomb.json";
		break;

	case EFFECT_PRE_EXPLOSION:
		rtn = "PreExplosion.json";
		break;

	case EFFECT_SLASHING_HIT:
		rtn = "SlashingHit.json";
		break;
		
	case EFFECT_DASH_SMOKE:
		rtn = "SmokeDash.json";
		break;
		
	case EFFECT_EXCLAMATION:
		rtn = "Exclamation.json";
		break;
	default:
		DEBUG_ASSERT( 0,  "エフェクト種類が想定外" );
		// とりあえず一番無難なものをセット
		rtn = "EffectBomb.json";
		break;
	}
	return rtn;
}







/* ====================================================================== */
/**
 * @brief  当たり判定付きのエフェクトクラス
 *
 * @note
 *		
 */
/* ====================================================================== */
GameEffectWithCollision *GameEffectWithCollision::CreateEffect( const Common::OWNER_TYPE &owner, const EFFECT_KIND &kind, const math::Vector2 &pos )
{
	return NEW GameEffectWithCollision( owner, kind, pos );
}

GameEffectWithCollision *GameEffectWithCollision::CreateEffect( const Common::OWNER_TYPE &owner, const EFFECT_KIND &kind, const int32_t &posX, const int32_t &posY )
{
	return NEW GameEffectWithCollision( owner, kind, math::Vector2( static_cast<float>(posX), static_cast<float>(posY) ) );
}

GameEffectWithCollision::GameEffectWithCollision( const Common::OWNER_TYPE &owner, const EFFECT_KIND &kind, const math::Vector2 &pos )
: TaskUnit("GameEffect")
, m_ownerType( owner )
, m_kind( kind )
{
	std::string readFileStr = SelectEffectFile();

	// 描画クラスセットアップ
	m_drawTexture.Init();
	m_drawTexture.m_pTex2D = Game2DBase::Create( readFileStr.c_str() );
	m_drawTexture.m_pTex2D->UpdateDrawInfo().m_fileName = readFileStr;
	m_drawTexture.m_pTex2D->UpdateDrawInfo().m_posOrigin = pos;
	m_drawTexture.m_pTex2D->UpdateDrawInfo().m_prioity = Common::PRIORITY_ABOVE_NORMAL;
}

GameEffectWithCollision::~GameEffectWithCollision(void)
{
}

bool GameEffectWithCollision::Init()
{
	// エフェクトに適した効果音再生
	switch( m_kind ){
	default:
		break;
	case EFFECT_EXPLOSION:
		SoundManager::GetInstance()->PlaySE( "explosion" );
		break;
	}

	return true;
}

void GameEffectWithCollision::Update()
{
}

void GameEffectWithCollision::DrawUpdate()
{
	if( std::string("").compare(m_drawTexture.m_pTex2D->GetPlayAnim()) == 0 ){
		// アニメは終わったので自殺
		TaskStartDie();
		return;
	}

	m_drawTexture.m_pTex2D->DrawUpdate2D();
}

const Common::TYPE_OBJECT GameEffectWithCollision::GetTypeObject() const
{
	Common::TYPE_OBJECT retType = Common::TYPE_MAX;
	switch( m_kind ){

	default:
		DEBUG_ASSERT( 0,  "エフェクト種類が想定外" );
		/* fall-through */

	case EFFECT_EXPLOSION:
		retType = ( m_ownerType == Common::OWNER_PLAYER ) ? Common::TYPE_EXPLOSION_PLAYER : Common::TYPE_EXPLOSION_ENEMY ;
		break;
	}
	return retType;
}

std::string GameEffectWithCollision::SelectEffectFile() const
{
	std::string rtn = "";
	switch(m_kind){
	case EFFECT_EXPLOSION:
		rtn = "Explosion.json";
		break;

	default:
		DEBUG_ASSERT( 0,  "エフェクト種類が想定外" );
		// とりあえず一番無難なものをセット
		rtn = "Explosion.json";
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
	: TaskUnit("GameEffectWithCollision")
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
	basePos.x += Utility::GetRandamValueFloat( 40, -40 );
	basePos.y += Utility::GetRandamValueFloat( 40, -40 );

	for(;damageInfo.m_array2D.size() < digitNum;){
		Texture2D tex;
		tex.Init();
		tex.m_pTex2D = Game2DBase::Create("DamageNum.json");
		const TEX_INIT_INFO &texInfo = TextureResourceManager::GetInstance()->GetLoadTextureInfo("DamageNum.json");
		basePos.x -= texInfo.m_sizeWidth;
		tex.m_pTex2D->UpdateDrawInfo().m_posOrigin	= basePos;
		tex.m_pTex2D->UpdateDrawInfo().m_prioity	= Common::PRIORITY_HIGH;
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
			Game2DBase *pTex2D = m_damageArray.at(i).m_array2D.at(j).m_pTex2D;
			pTex2D->UpdateDrawInfo().m_posOrigin.y -= static_cast<float>( 3 );
			pTex2D->DrawUpdate2D();
		}
	}
}