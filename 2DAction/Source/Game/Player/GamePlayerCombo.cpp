/* ====================================================================== */
/**
 * @brief  プレイヤーのコンボ表示を行う
 *
 * @note
 *		
 */
/* ====================================================================== */

#include "System/picojson.h"
#include "Game/GameRecorder.h"
#include "GamePlayerCombo.h"
#include "Common/Utility/CommonGameUtility.h"
#include "System/Draw2D/SystemDraw2DResource.h"

PlayerCombo *PlayerCombo::CreatePlayerCombo()
{
	return NEW PlayerCombo();
}

PlayerCombo::PlayerCombo()
: TaskUnit( "PlayerCombo" )
, m_pNumCounterCombo( NULL )
, m_pNumCounterComboYellow( NULL )
, m_pNumCounterComboRed( NULL )
{
	m_textureFrame.Init();
	m_textureCombo.Init();
	m_textureComboGauge.Init();

	std::string jsonStr = "ComboFrame.json";
	m_textureFrame.m_pTex2D = Game2DBase::Create( jsonStr.c_str() );

	//!初期位置セット
	m_textureFrame.m_texInfo.m_prioity = PRIORITY_ABOVE_NORMAL;
	m_textureFrame.m_texInfo.m_fileName = jsonStr;
	const TEX_INIT_INFO &texInfo = TextureResourceManager::GetInstance()->GetLoadTextureInfo( jsonStr.c_str() );
	m_textureFrame.m_texInfo.m_posOrigin = math::Vector2( WINDOW_WIDTH - static_cast<float>(texInfo.m_sizeWidth/2), WINDOW_HEIGHT - static_cast<float>(texInfo.m_sizeHeight/2) );
	m_textureFrame.m_texInfo.m_usePlayerOffset = false;
	m_textureFrame.m_pTex2D->SetDrawInfo(m_textureFrame.m_texInfo);
}

PlayerCombo::~PlayerCombo(void)
{
}

bool PlayerCombo::DieMain()
{
	m_textureFrame.DeleteAndInit();
	m_textureCombo.DeleteAndInit();
	m_textureComboGauge.DeleteAndInit();
	return true;
}

bool PlayerCombo::Init()
{
	// ステータスメニューのパーツ情報取得
	Utility::GetPartsInfoFromJson( m_textureFrame.m_texInfo.m_fileName, m_partsMap );

	// 各種パーツセット
	m_textureCombo.Init();
	m_textureCombo.m_pTex2D = Game2DBase::Create("ComboHitStr.json");
	m_textureCombo.m_texInfo.m_fileName = "ComboHitStr.json";
	m_textureCombo.m_texInfo.m_prioity = PRIORITY_HIGH;
	m_textureCombo.m_texInfo.m_posOrigin = GetPartsPos("comboStr");
	m_textureCombo.m_texInfo.m_usePlayerOffset = false;
	m_textureCombo.m_pTex2D->SetDrawInfo(m_textureCombo.m_texInfo);
	
	// コンボ数
	m_pNumCounterCombo = NumberCounter::Create("NumberLarge.json");
	m_pNumCounterComboYellow = NumberCounter::Create("NumberLargeYellow.json");
	m_pNumCounterComboRed = NumberCounter::Create("NumberLargeRed.json");
	TEX_DRAW_INFO comboInfo;
	comboInfo.Init();
	comboInfo.m_prioity = PRIORITY_HIGH;
	comboInfo.m_posOrigin = GetPartsPos("count");
	comboInfo.m_usePlayerOffset = false;
	m_pNumCounterCombo->SetDrawInfo( comboInfo );
	m_pNumCounterComboYellow->SetDrawInfo( comboInfo );
	m_pNumCounterComboRed->SetDrawInfo( comboInfo );

	// コンボの継続時間を表すコンボゲージ
	m_textureComboGauge.Init();
	m_textureComboGauge.m_pTex2D = Game2DBase::Create("ComboGauge.json");
	m_textureComboGauge.m_texInfo.m_fileName = "ComboGauge.json";
	m_textureComboGauge.m_texInfo.m_prioity = PRIORITY_HIGH;
	m_textureComboGauge.m_texInfo.m_posOrigin = GetPartsPos("comboGauge");
	m_textureComboGauge.m_texInfo.m_usePlayerOffset = false;
	const TEX_INIT_INFO &texInfoComboGauge = TextureResourceManager::GetInstance()->GetLoadTextureInfo( m_textureComboGauge.m_texInfo.m_fileName.c_str() );
	m_textureComboGauge.m_texInfo.m_arrangeOrigin = math::Vector2( static_cast<float>(texInfoComboGauge.m_sizeWidth), 0.0f );
	m_textureComboGauge.m_pTex2D->SetDrawInfo(m_textureCombo.m_texInfo);

	return true;
}

/* ================================================ */
/**
 * @brief	各種Update関数
 */
/* ================================================ */
void PlayerCombo::Update()
{
	// 現在のコンボ数によって表示する文字色変化	
	uint32_t counter = GameRecorder::GetInstance()->GetCurrentHitCounter();
	if( m_pNumCounterCombo && m_pNumCounterComboYellow && m_pNumCounterComboRed ){
		m_pNumCounterCombo->SetValue( counter );		
		m_pNumCounterComboYellow->SetValue( counter );
		m_pNumCounterComboRed->SetValue( counter );
		if( counter < 30 ){
			m_pNumCounterCombo->SetDrawInvalidFlag( false );		
			m_pNumCounterComboYellow->SetDrawInvalidFlag( true );
			m_pNumCounterComboRed->SetDrawInvalidFlag( true );
		}
		else if( counter < 70 ){
			m_pNumCounterCombo->SetDrawInvalidFlag( true );		
			m_pNumCounterComboYellow->SetDrawInvalidFlag( false );
			m_pNumCounterComboRed->SetDrawInvalidFlag( true );
		}
		else{
			m_pNumCounterCombo->SetDrawInvalidFlag( true );		
			m_pNumCounterComboYellow->SetDrawInvalidFlag( true );
			m_pNumCounterComboRed->SetDrawInvalidFlag( false );
		}
	}
}

void PlayerCombo::DrawUpdate()
{
	if( m_textureFrame.m_pTex2D ){
		// ステータス外枠描画
		m_textureFrame.m_pTex2D->DrawUpdate2D();

		// コンボ文字列
		m_textureCombo.m_pTex2D->DrawUpdate2D();
	}
	if( m_textureComboGauge.m_pTex2D ){
		// コンボ持続時間ゲージ
		uint32_t leftTime = GameRecorder::GetInstance()->GetLeftTimeOfCombo();
		float scale = 10.0f;
		scale = 10.0f * ( static_cast<float>(leftTime) / static_cast<float>(GameRecorder::COMBO_COUNT_MAX) );
		m_textureComboGauge.m_texInfo.m_scale.x = scale;
		m_textureComboGauge.m_pTex2D->SetDrawInfo( m_textureComboGauge.m_texInfo );
		m_textureComboGauge.m_pTex2D->DrawUpdate2D();
	}
}

const math::Vector2 PlayerCombo::GetPartsPos( const std::string name ) const
{
	// ステータスメニューの左上座標取得
	const TEX_INIT_INFO &statusMenuInfo = TextureResourceManager::GetInstance()->GetLoadTextureInfo( m_textureFrame.m_texInfo.m_fileName.c_str() );
	math::Vector2 retPos = m_textureFrame.m_texInfo.m_posOrigin;
	retPos -= math::Vector2( statusMenuInfo.m_sizeWidth / 2.0f, statusMenuInfo.m_sizeHeight / 2.0f );

	// そこからパーツの位置を足し算
	Common::PARTS_INFO info = GetPartsInfo(name);
	retPos += info.m_pos;
	return retPos;
}

const Common::PARTS_INFO &PlayerCombo::GetPartsInfo( const std::string name ) const
{
	auto it = m_partsMap.find( name.c_str() );
	if( it != m_partsMap.end() ){
		return (*it).second;
	}
	DEBUG_ASSERT( 0, "パーツが見つかりません\n" );
	return (*m_partsMap.begin()).second;
}



