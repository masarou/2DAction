/* ====================================================================== */
/**
 * @brief  ゲーム画面左下のプレイヤーステータス画面
 *
 * @note
 *		
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
: MenuWindow( "MenuPlayerStatus.json", Common::PRIORITY_HIGH, math::Vector2( 240, WINDOW_HEIGHT - 35 ) )
, m_playerLife( 0 )
, m_playerLifeMax( 0 )
, m_itemLevelDamage( 0 )
, m_itemLevelRapid( 0 )
{
}

PlayerStatusMenu::~PlayerStatusMenu(void)
{
}

bool PlayerStatusMenu::InitMenu()
{
	// 各種パーツセット
	TEX_DRAW_INFO drawInfo;
	m_lifeGauge.Init();
	m_lifeGauge.m_pTex2D = Game2DBase::Create("LifeGauge.json");
	drawInfo.m_fileName = "LifeGauge.json";
	drawInfo.m_prioity = Common::PRIORITY_HIGH;
	drawInfo.m_posOrigin = math::Vector2( 22, WINDOW_HEIGHT - 24 );
	drawInfo.m_usePlayerOffset = false;
	drawInfo.m_arrangeOrigin = math::Vector2( 0.0f, 0.0f );	// 拡大縮小する画像なのでセットしておく
	m_lifeGauge.m_pTex2D->SetDrawInfo( drawInfo );

	// カウントアップアニメは行わない
	PartsCounter *pPartsHPCurr = GetPartsCounter("lifeCurr");
	PartsCounter *pPartsHPMax = GetPartsCounter("lifeMax");
	if( pPartsHPCurr && pPartsHPMax ){
		pPartsHPCurr->SetCountAnimFlg( false );
		pPartsHPMax->SetCountAnimFlg( false );
	}

	return true;
}

void PlayerStatusMenu::UpdateMenu()
{
	// 現在のHP描画
	PartsCounter *pPartsHPCurr = GetPartsCounter("lifeCurr");
	PartsCounter *pPartsHPMax = GetPartsCounter("lifeMax");
	if( pPartsHPCurr && pPartsHPMax ){
		pPartsHPCurr->SetValue( m_playerLife );
		pPartsHPMax->SetValue( m_playerLifeMax );
	}
}

void PlayerStatusMenu::DrawUpdateMenu()
{
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
}

bool PlayerStatusMenu::DieMainMenu()
{
	m_lifeGauge.DeleteAndInit();
	return true;
}

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
