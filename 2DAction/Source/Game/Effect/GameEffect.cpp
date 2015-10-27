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

GameEffect *GameEffect::CreateEffect( const EFFECT_KIND &kind, const int32_t &posX, const int32_t &posY )
{
	return CreateEffect( kind, math::Vector2( static_cast<float>(posX), static_cast<float>(posY) ) );
}

GameEffect *GameEffect::CreateEffect( const EFFECT_KIND &kind, const math::Vector2 &pos )
{
	GameEffect *pEffect = NULL;
	if( kind == EFFECT_PRE_FIRE_WALL ){
		pEffect = NEW PreFireWall( kind, pos );
	}
	else{
		pEffect = NEW GameEffect( kind, pos );
	}
	return pEffect;
}

void GameEffect::SetEffectAnim( const std::string &animStr )
{
	if( m_textureEffect.m_pTex2D ){
		m_textureEffect.m_pTex2D->SetAnim( animStr );
	}
}

GameEffect::GameEffect( const EFFECT_KIND &kind, const math::Vector2 &pos )
	: TaskUnit("GameEffect")
	, m_isInvalid( false )
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
		uint32_t rotate = Utility::GetRandamValue( static_cast<uint32_t>( math::ANGLE_FULL ), 0 );
		drawInfo.m_rot = static_cast<float>(rotate);
	}
	if( m_kind == EFFECT_DASH_SMOKE ){
		drawInfo.m_prioity = Common::PRIORITY_BELOW_NORMAL;
	}
	if( m_kind == EFFECT_WORP ){
		drawInfo.m_posOrigin.y -= 150.0f;
	}
	if( m_kind == EFFECT_FIRE_WALL ){
		drawInfo.m_usePlayerOffset = false;
	}
	if( m_kind == EFFECT_PRE_FIRE_WALL ){
		drawInfo.m_drawForce = true;
	}
	if( m_kind == EFFECT_SUMMON ){
		drawInfo.m_prioity = Common::PRIORITY_BELOW_NORMAL;
	}
	if( m_kind == EFFECT_FIRE_FLOWER ){
		drawInfo.m_usePlayerOffset = false;
	}
	if( m_kind == EFFECT_HEALING ){
		drawInfo.m_usePlayerOffset = false;
		drawInfo.m_posOrigin.y -= 40.0f;
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
	if( m_kind == EFFECT_FIRE_FLOWER ){
		std::string animStr;
		animStr += '0' + Utility::GetRandamValue( 6, 0 );
		m_textureEffect.m_pTex2D->SetAnim( animStr.c_str() );
	}
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

		// 親がいれば終了メッセージ投げる
		Message msg = Message( EFFECT_ANIM_END );
		SendMessageToParent( msg );

		if( m_kind == EFFECT_PRE_EXPLOSION ){
			GameEffectWithCollision::CreateEffect( Common::OWNER_ENEMY, GameEffectWithCollision::EFFECT_EXPLOSION, m_textureEffect.m_pTex2D->GetDrawInfo().m_posOrigin );
		}
		return;
	}

	if( !m_isInvalid ){
		m_textureEffect.m_pTex2D->DrawUpdate2D();
	}
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

	case EFFECT_INVALID_DAMAGE:
		rtn = "InvalidDamage.json";
		break;

	case EFFECT_WORP:
		rtn = "WorpEffect.json";
		break;
		
	case EFFECT_PRE_FIRE_WALL:
		rtn = "PreFireWall.json";
		break;

	case EFFECT_FIRE_WALL:
		rtn = "FireWall.json";
		break;

	case EFFECT_SUMMON:
		rtn = "Summon.json";
		break;

	case EFFECT_FIRE_FLOWER:
		rtn = "FireFlower.json";
		break;
		
	case EFFECT_HEALING:
		rtn = "EffectHeal.json";
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
 * @brief	画面全体炎演出前に出す予兆エフェクト
 *
 * @note
 *
 */
/* ====================================================================== */
static const uint32_t FIRE_WALL_EFFECT_TIME = 110;
PreFireWall::PreFireWall( const EFFECT_KIND &kind, const math::Vector2 &pos )
: GameEffect( kind, pos )
, m_liveTime( 0 )
{
}

PreFireWall::~PreFireWall()
{
}

void PreFireWall::Update()
{
	++m_liveTime;

	TEX_DRAW_INFO &texInfo = m_textureEffect.m_pTex2D->UpdateDrawInfo();
	texInfo.m_scale *= 1.05f;

	// 一定時間たったら自殺
	if( m_liveTime >= FIRE_WALL_EFFECT_TIME ){
		TaskStartDie();
	}
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
	GameEffectWithCollision *effect = NULL;

	if( kind == EFFECT_FIRE ){
		effect = NEW FireWithCollision( owner, kind, pos );
	}
	else{
		effect = NEW GameEffectWithCollision( owner, kind, pos );
	}

	return effect;
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
		SoundManager::GetInstance()->PlaySE( "Explosion" );
		break;
	case EFFECT_FIRE:
		break;
	}

	return true;
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
	case EFFECT_FIRE:
		retType = Common::TYPE_FIRE;
		break;
	case EFFECT_POISON:
		retType = Common::TYPE_POISON;
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
		
	case EFFECT_FIRE:
		rtn = "Fire.json";
		break;
		
	case EFFECT_POISON:
		rtn = "Poison.json";
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
 * @brief	火の玉の後に残る炎エフェクト
 *
 * @note
 *			FIRE_LIVE_TIMEの時間経過後自殺
 */
/* ====================================================================== */
// 炎エフェクト継続時間
#define FIRE_LIVE_TIME 60*30

FireWithCollision::FireWithCollision( const Common::OWNER_TYPE &owner, const EFFECT_KIND &kind, const math::Vector2 &pos )
: GameEffectWithCollision( owner, kind, pos )
, m_liveTime( 0 )
{
}

FireWithCollision::~FireWithCollision()
{
}

void FireWithCollision::Update()
{
	++m_liveTime;

	// 一定時間たったら自殺
	if( m_liveTime >= FIRE_LIVE_TIME ){
		TaskStartDie();
	}
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

void GameEffectDamage::CreateEffectDamage( const uint32_t &value, const int32_t &posX, const int32_t &posY, bool isPlayer )
{
	EFFECT_DAMAGE_INFO damageInfo;
	damageInfo.Init();
	damageInfo.m_value = value;

	std::string readFileName = ( isPlayer ) ? "DamageNumPlayer.json" : "DamageNum.json" ;

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
		tex.m_pTex2D = Game2DBase::Create( readFileName.c_str() );
		const TEX_INIT_INFO &texInfo = TextureResourceManager::GetInstance()->GetLoadTextureInfo( readFileName.c_str() );
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

	// valueが0 = ダメージなしだった時の特殊処理
	if( value == 0 ){
		Texture2D tex;
		tex.Init();
		tex.m_pTex2D = Game2DBase::Create( readFileName.c_str() );
		const TEX_INIT_INFO &texInfo = TextureResourceManager::GetInstance()->GetLoadTextureInfo( readFileName.c_str() );
		basePos.x -= texInfo.m_sizeWidth;
		tex.m_pTex2D->UpdateDrawInfo().m_posOrigin	= basePos;
		tex.m_pTex2D->UpdateDrawInfo().m_prioity	= Common::PRIORITY_HIGH;
		damageInfo.m_array2D.push_back( tex );
		damageInfo.m_array2D.at(0).m_pTex2D->SetAnim( "0" );
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








GameEffectLoop *GameEffectLoop::CreateEffect( const EFFECT_KIND &kind, const int32_t &posX, const int32_t &posY )
{
	return CreateEffect( kind, math::Vector2( static_cast<float>(posX), static_cast<float>(posY) ) );
}

GameEffectLoop *GameEffectLoop::CreateEffect( const EFFECT_KIND &kind, const math::Vector2 &pos )
{
	GameEffectLoop *pEffect = NULL;
	pEffect = NEW GameEffectLoop( kind, pos );
	return pEffect;
}

void GameEffectLoop::SetEffectAnim( const std::string &animStr )
{
	if( m_textureEffect.m_pTex2D ){
		m_textureEffect.m_pTex2D->SetAnim( animStr );
	}
}

GameEffectLoop::GameEffectLoop( const EFFECT_KIND &kind, const math::Vector2 &pos )
	: TaskUnit("GameEffect")
	, m_isInvalid( false )
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
	m_textureEffect.m_pTex2D->SetDrawInfo( drawInfo );
}

GameEffectLoop::~GameEffectLoop(void)
{
}

bool GameEffectLoop::DieMain()
{
	m_textureEffect.DeleteAndInit();
	return true;
}

bool GameEffectLoop::Init()
{
	return true;
}

void GameEffectLoop::Update()
{
	if( m_kind == EFFECT_STATUS_ICON ){
		TEX_DRAW_INFO &info = m_textureEffect.m_pTex2D->UpdateDrawInfo();
		math::Vector2 iconPos = Utility::GetPlayerPos();
		iconPos.y -= 40.0f;
		info.m_posOrigin = iconPos;
	}
}

void GameEffectLoop::DrawUpdate()
{
	if( !m_isInvalid ){
		m_textureEffect.m_pTex2D->DrawUpdate2D();
	}
}

std::string GameEffectLoop::SelectEffectFile() const
{
	std::string rtn = "";
	switch(m_kind){

	case EFFECT_STATUS_ICON:
		rtn = "StatusIcon.json";
		break;

	default:
		DEBUG_ASSERT( 0,  "エフェクト種類が想定外" );
		// とりあえず一番無難なものをセット
		rtn = "EffectBomb.json";
		break;
	}
	return rtn;
}