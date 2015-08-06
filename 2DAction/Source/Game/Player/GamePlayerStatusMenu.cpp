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
	m_textureStatus.m_pTex2D = Game2DBase::Create( jsonStr.c_str() );

	//!初期位置セット
	TEX_DRAW_INFO drawInfoStatus;
	drawInfoStatus.m_prioity = PRIORITY_ABOVE_NORMAL;
	drawInfoStatus.m_fileName = jsonStr;
	drawInfoStatus.m_posOrigin = math::Vector2( 240, WINDOW_HEIGHT - 35 );
	drawInfoStatus.m_usePlayerOffset = false;
	m_textureStatus.m_pTex2D->SetDrawInfo( drawInfoStatus );
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
	if( !m_textureStatus.m_pTex2D ){
		DEBUG_ASSERT( 0, "必要なクラスが作られていない" );
		return true;
	}

	// ステータスメニューのパーツ情報取得
	Utility::GetPartsInfoFromJson( m_textureStatus.m_pTex2D->GetDrawInfo().m_fileName, m_partsMap );

	// 各種パーツセット
	TEX_DRAW_INFO drawInfo;
	m_lifeGauge.Init();
	m_lifeGauge.m_pTex2D = Game2DBase::Create("LifeGauge.json");
	drawInfo.m_fileName = "LifeGauge.json";
	drawInfo.m_prioity = PRIORITY_HIGH;
	drawInfo.m_posOrigin = GetPartsPos("lifeBarPos");
	drawInfo.m_usePlayerOffset = false;
	drawInfo.m_arrangeOrigin = math::Vector2( 0.0f, 0.0f );	// 拡大縮小する画像なのでセットしておく
	m_lifeGauge.m_pTex2D->SetDrawInfo( drawInfo );

	// 連射速度レベル
	drawInfo.Init();
	m_rapidLevel.Init();
	m_rapidLevel.m_pTex2D = Game2DBase::Create("NumberRitch.json");
	drawInfo.m_fileName = "NumberRitch.json";
	drawInfo.m_prioity = PRIORITY_HIGH;
	drawInfo.m_posOrigin = GetPartsPos("itemLevel01");
	drawInfo.m_usePlayerOffset = false;
	m_rapidLevel.m_pTex2D->SetDrawInfo( drawInfo );

	// 攻撃力レベル
	drawInfo.Init();
	m_danageLevel.Init();
	m_danageLevel.m_pTex2D = Game2DBase::Create("NumberRitch.json");
	drawInfo.m_fileName = "NumberRitch.json";
	drawInfo.m_prioity = PRIORITY_HIGH;
	drawInfo.m_posOrigin = GetPartsPos("itemLevel02");
	drawInfo.m_usePlayerOffset = false;
	m_danageLevel.m_pTex2D->SetDrawInfo( drawInfo );

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
			TEX_DRAW_INFO &drawInfo = m_lifeGauge.m_pTex2D->UpdateDrawInfo();
			const TEX_INIT_INFO &lifeTexInfo = TextureResourceManager::GetInstance()->GetLoadTextureInfo( drawInfo.m_fileName.c_str() );
			float ratio = static_cast<float>(m_playerLife)/static_cast<float>(m_playerLifeMax);
			float lifeValue = ( 450 / lifeTexInfo.m_sizeWidth) * ratio; // 450はライフバーの長さ

			if( math::Absf( drawInfo.m_scale.x - lifeValue ) > 0.3f ){
				drawInfo.m_scale.x *= (drawInfo.m_scale.x - lifeValue < 0.0f) ? 1.02f : 0.98f ;
			}
			else{
				drawInfo.m_scale.x = lifeValue;
			}
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
	if( !m_textureStatus.m_pTex2D ){
		DEBUG_ASSERT( 0, "必要なクラスが作られていない" );
		return math::Vector2();
	}

	// ステータスメニューの左上座標取得
	const TEX_INIT_INFO &statusMenuInfo = TextureResourceManager::GetInstance()->GetLoadTextureInfo( m_textureStatus.m_pTex2D->GetDrawInfo().m_fileName.c_str() );
	math::Vector2 retPos = m_textureStatus.m_pTex2D->GetDrawInfo().m_posOrigin;
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



