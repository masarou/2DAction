/* ====================================================================== */
/**
 * @brief  GamePlayerAttackGunから発射される球クラス
 *
 * @note
 *		GamePlayerAttackGunを親として発射された球のクラス
 *		一発の玉の情報を持ち、スピードや方向、位置情報を持つ
 *		
 */
/* ====================================================================== */

#include "System/picojson.h"
#include "GamePlayerStatusMenu.h"
#include "Common/Utility/CommonGameUtility.h"
#include "System/Draw2D/SystemDraw2DResource.h"

// 固定値
static uint32_t ITEM_LEVEL_MAX = 9;

PlayerStatusMenu::PlayerStatusMenu()
: TaskUnit( "MenuPlayerStatus" )
, m_playerLife( 0 )
, m_playerLifeMax( 0 )
, m_itemLevelDamage( 0 )
, m_itemLevelRapid( 0 )
{
	m_textureStatus.Init();
	m_lifeGauge.Init();
	m_danageLevel.Init();
	m_rapidLevel.Init();

	std::string jsonStr = "PlayerStatus.json";
	m_textureStatus.m_pTex2D = NEW Game2DBase( jsonStr.c_str() );

	//!初期位置セット
	m_textureStatus.m_texInfo.m_prioity = PRIORITY_ABOVE_NORMAL;
	m_textureStatus.m_texInfo.m_fileName = jsonStr;
	m_textureStatus.m_texInfo.m_posOrigin = math::Vector2( 240, WINDOW_HEIGHT - 35 );
	m_textureStatus.m_texInfo.m_usePlayerOffset = false;
	m_textureStatus.m_pTex2D->SetDrawInfo(m_textureStatus.m_texInfo);
}

PlayerStatusMenu::~PlayerStatusMenu(void)
{
}

bool PlayerStatusMenu::DieMain()
{
	m_textureStatus.DeleteAndInit();
	m_lifeGauge.DeleteAndInit();
	m_danageLevel.DeleteAndInit();
	m_rapidLevel.DeleteAndInit();
	return true;
}

bool PlayerStatusMenu::Init()
{
	// ステータスメニューのパーツ情報取得
	Utility::GetPartsInfoFromJson( m_textureStatus.m_texInfo.m_fileName, m_partsMap );

	// 各種パーツセット
	m_lifeGauge.Init();
	m_lifeGauge.m_pTex2D = NEW Game2DBase("LifeGauge.json");
	m_lifeGauge.m_texInfo.m_fileName = "LifeGauge.json";
	m_lifeGauge.m_texInfo.m_prioity = PRIORITY_HIGH;
	m_lifeGauge.m_texInfo.m_posOrigin = GetPartsPos("lifeBarPos");
	m_lifeGauge.m_texInfo.m_usePlayerOffset = false;
	m_lifeGauge.m_texInfo.m_arrangeOrigin = math::Vector2( 0.0f, 0.0f );	// 拡大縮小する画像なのでセットしておく
	m_lifeGauge.m_pTex2D->SetDrawInfo(m_lifeGauge.m_texInfo);

	// 連射速度レベル
	m_rapidLevel.Init();
	m_rapidLevel.m_pTex2D = NEW Game2DBase("NumberLv.json");
	m_rapidLevel.m_texInfo.m_fileName = "NumberLv.json";
	m_rapidLevel.m_texInfo.m_prioity = PRIORITY_HIGH;
	m_rapidLevel.m_texInfo.m_posOrigin = GetPartsPos("itemLevel01");
	m_rapidLevel.m_texInfo.m_usePlayerOffset = false;
	m_rapidLevel.m_pTex2D->SetDrawInfo(m_rapidLevel.m_texInfo);

	// 攻撃力レベル
	m_danageLevel.Init();
	m_danageLevel.m_pTex2D = NEW Game2DBase("NumberLv.json");
	m_danageLevel.m_texInfo.m_fileName = "NumberLv.json";
	m_danageLevel.m_texInfo.m_prioity = PRIORITY_HIGH;
	m_danageLevel.m_texInfo.m_posOrigin = GetPartsPos("itemLevel02");
	m_danageLevel.m_texInfo.m_usePlayerOffset = false;
	m_danageLevel.m_pTex2D->SetDrawInfo(m_danageLevel.m_texInfo);

	return true;
}

/* ================================================ */
/**
 * @brief	現在表示しているアイテムレベル取得
 */
/* ================================================ */
void PlayerStatusMenu::AddItemLevel( Common::ITEM_KIND kind )
{
	switch( kind ){
	default:
		break;
	case Common::ITEM_KIND_DAMAGE_UP:
		if( m_itemLevelDamage < ITEM_LEVEL_MAX ){
			++m_itemLevelDamage;
		}
		break;
		
	case Common::ITEM_KIND_RAPID_BULLET:
		if( m_itemLevelRapid < ITEM_LEVEL_MAX ){
			++m_itemLevelRapid;
		}
		break;
	}
}

/* ================================================ */
/**
 * @brief	現在表示しているアイテムレベル取得
 */
/* ================================================ */
const uint32_t PlayerStatusMenu::GetItemLevel( const Common::ITEM_KIND &kind ) const
{
	uint32_t retval = 0;
	switch( kind ){
	default:
		DEBUG_ASSERT( 0, "アイテムの種類が想定外" );
		break;
	case Common::ITEM_KIND_DAMAGE_UP:
		retval = m_itemLevelDamage;
		break;
		
	case Common::ITEM_KIND_RAPID_BULLET:
		retval = m_itemLevelRapid;
		break;
	}
	return retval;
}

/* ================================================ */
/**
 * @brief	各種Update関数
 */
/* ================================================ */
void PlayerStatusMenu::SetPlayerHP( const uint32_t &val, const uint32_t &lifeMax )
{
	m_playerLife = val;
	m_playerLifeMax = lifeMax;
}

/* ================================================ */
/**
 * @brief	各種Update関数
 */
/* ================================================ */
void PlayerStatusMenu::Update()
{
}

void PlayerStatusMenu::DrawUpdate()
{
	if( m_textureStatus.m_pTex2D ){
		// ステータス外枠描画
		m_textureStatus.m_pTex2D->DrawUpdate2D();

		// ライフゲージ描画
		if( m_lifeGauge.m_pTex2D ){
			const TEX_INIT_INFO &lifeTexInfo = TextureResourceManager::GetInstance()->GetLoadTextureInfo( m_lifeGauge.m_texInfo.m_fileName.c_str() );
			float ratio = static_cast<float>(m_playerLife)/static_cast<float>(m_playerLifeMax);
			float lifeValue = ( 450 / lifeTexInfo.m_sizeWidth) * ratio; // 450はライフバーの長さ
		
			if( math::Absf( m_lifeGauge.m_texInfo.m_scale.x - lifeValue ) > 0.3f ){
				m_lifeGauge.m_texInfo.m_scale.x *= (m_lifeGauge.m_texInfo.m_scale.x - lifeValue < 0.0f) ? 1.02f : 0.98f ;
			}
			else{
				m_lifeGauge.m_texInfo.m_scale.x = lifeValue;
			}
			m_lifeGauge.m_pTex2D->SetDrawInfo(m_lifeGauge.m_texInfo);
			m_lifeGauge.m_pTex2D->DrawUpdate2D();
		}

		// 攻撃、連射速度レベル描画
		if( m_danageLevel.m_pTex2D ){
			std::string lv = std::to_string( static_cast<long double>(m_itemLevelDamage) );
			m_danageLevel.m_pTex2D->SetAnim( lv );
			m_danageLevel.m_pTex2D->DrawUpdate2D();
		}
		if( m_rapidLevel.m_pTex2D ){
			std::string lv = std::to_string( static_cast<long double>(m_itemLevelRapid) );
			m_rapidLevel.m_pTex2D->SetAnim( lv );
			m_rapidLevel.m_pTex2D->DrawUpdate2D();
		}
	}
}

const math::Vector2 PlayerStatusMenu::GetPartsPos( const std::string name ) const
{
	// ステータスメニューの左上座標取得
	const TEX_INIT_INFO &statusMenuInfo = TextureResourceManager::GetInstance()->GetLoadTextureInfo( m_textureStatus.m_texInfo.m_fileName.c_str() );
	math::Vector2 retPos = m_textureStatus.m_texInfo.m_posOrigin;
	retPos -= math::Vector2( statusMenuInfo.m_sizeWidth / 2.0f, statusMenuInfo.m_sizeHeight / 2.0f );

	// そこからパーツの位置を足し算
	Common::PARTS_INFO info = GetPartsInfo(name);
	retPos += info.m_pos;
	return retPos;
}

const Common::PARTS_INFO &PlayerStatusMenu::GetPartsInfo( const std::string name ) const
{
	auto it = m_partsMap.find( name.c_str() );
	if( it != m_partsMap.end() ){
		return (*it).second;
	}
	DEBUG_ASSERT( 0, "パーツが見つかりません\n" );
	return (*m_partsMap.begin()).second;
}



