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
#include "Flow/FlowManager.h"

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
	TEX_DRAW_INFO drawInfo;
	drawInfo.m_prioity = Common::PRIORITY_ABOVE_NORMAL;
	drawInfo.m_fileName = jsonStr;
	const TEX_INIT_INFO &texInfo = TextureResourceManager::GetInstance()->GetLoadTextureInfo( jsonStr.c_str() );
	drawInfo.m_posOrigin = math::Vector2( WINDOW_WIDTH - static_cast<float>(texInfo.m_sizeWidth/2), WINDOW_HEIGHT - static_cast<float>(texInfo.m_sizeHeight/2) );
	drawInfo.m_usePlayerOffset = false;
	m_textureFrame.m_pTex2D->SetDrawInfo( drawInfo );
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

/* ================================================ */
/**
 * @brief	初期化
 */
/* ================================================ */
bool PlayerCombo::Init()
{
	if( !m_textureFrame.m_pTex2D ){
		DEBUG_ASSERT( 0, "必要なクラスが作られていない" );
		return true;
	}

	// ステータスメニューのパーツ情報取得
	Utility::GetPartsInfoFromJson( m_textureFrame.m_pTex2D->GetDrawInfo().m_fileName, m_partsMap );

	// 各種パーツセット
	TEX_DRAW_INFO drawInfoCombo;
	m_textureCombo.Init();
	m_textureCombo.m_pTex2D = Game2DBase::Create("ComboHitStr.json");
	drawInfoCombo.m_fileName = "ComboHitStr.json";
	drawInfoCombo.m_prioity = Common::PRIORITY_HIGH;
	drawInfoCombo.m_posOrigin = GetPartsPos("comboStr");
	drawInfoCombo.m_usePlayerOffset = false;
	m_textureCombo.m_pTex2D->SetDrawInfo( drawInfoCombo );
	
	// コンボ数
	if( FlowManager::GetInstance()->GetCurrentFlowKind() == Common::FLOW_STAGE07
		|| FlowManager::GetInstance()->GetCurrentFlowKind() == Common::FLOW_STAGE08
		|| FlowManager::GetInstance()->GetCurrentFlowKind() == Common::FLOW_STAGE09
		|| FlowManager::GetInstance()->GetCurrentFlowKind() == Common::FLOW_STAGE10 ){
			m_pNumCounterCombo = NumberCounter::Create("NumberLargeWhite.json");
	}
	else{
		m_pNumCounterCombo = NumberCounter::Create("NumberLarge.json");
	}
	m_pNumCounterComboYellow = NumberCounter::Create("NumberLargeYellow.json");
	m_pNumCounterComboRed = NumberCounter::Create("NumberLargeRed.json");
	TEX_DRAW_INFO comboInfo;
	comboInfo.Init();
	comboInfo.m_prioity = Common::PRIORITY_HIGH;
	comboInfo.m_posOrigin = GetPartsPos("count");
	comboInfo.m_usePlayerOffset = false;
	m_pNumCounterCombo->SetDrawInfo( comboInfo );
	m_pNumCounterComboYellow->SetDrawInfo( comboInfo );
	m_pNumCounterComboRed->SetDrawInfo( comboInfo );

	// コンボの継続時間を表すコンボゲージ
	TEX_DRAW_INFO drawInfoGauge;
	m_textureComboGauge.Init();
	m_textureComboGauge.m_pTex2D = Game2DBase::Create("ComboGauge.json");
	drawInfoGauge.m_fileName = "ComboGauge.json";
	drawInfoGauge.m_prioity = Common::PRIORITY_HIGH;
	drawInfoGauge.m_posOrigin = GetPartsPos("comboGauge");
	drawInfoGauge.m_usePlayerOffset = false;
	const TEX_INIT_INFO &texInfoComboGauge = TextureResourceManager::GetInstance()->GetLoadTextureInfo( drawInfoGauge.m_fileName.c_str() );
	drawInfoGauge.m_arrangeOrigin = math::Vector2( static_cast<float>(texInfoComboGauge.m_sizeWidth), 0.0f );
	m_textureComboGauge.m_pTex2D->SetDrawInfo( drawInfoGauge );

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

	if( m_textureComboGauge.m_pTex2D ){
		// コンボ持続時間ゲージ
		uint32_t leftTime = GameRecorder::GetInstance()->GetLeftTimeOfCombo();
		float scale = 10.0f;
		scale = 10.0f * ( static_cast<float>(leftTime) / static_cast<float>(GameRecorder::COMBO_COUNT_MAX) );
		TEX_DRAW_INFO updateDrawInfo = m_textureComboGauge.m_pTex2D->GetDrawInfo();
		updateDrawInfo.m_scale.x = scale;
		m_textureComboGauge.m_pTex2D->SetDrawInfo( updateDrawInfo );
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
		m_textureComboGauge.m_pTex2D->DrawUpdate2D();
	}
}

/* ================================================ */
/**
 * @brief	指定パーツの位置情報取得
 */
/* ================================================ */
const math::Vector2 PlayerCombo::GetPartsPos( const std::string name ) const
{
	if( !m_textureFrame.m_pTex2D ){
		DEBUG_ASSERT( 0, "必要なクラスが作成されていない");
		return math::Vector2();
	}

	// ステータスメニューの左上座標取得
	const TEX_INIT_INFO &statusMenuInfo = TextureResourceManager::GetInstance()->GetLoadTextureInfo( m_textureFrame.m_pTex2D->GetDrawInfo().m_fileName.c_str() );
	math::Vector2 retPos = m_textureFrame.m_pTex2D->GetDrawInfo().m_posOrigin;
	retPos -= math::Vector2( statusMenuInfo.m_sizeWidth / 2.0f, statusMenuInfo.m_sizeHeight / 2.0f );

	// そこからパーツの位置を足し算
	Common::PARTS_INFO info = GetPartsInfo(name);
	retPos += info.m_pos;
	return retPos;
}

/* ================================================ */
/**
 * @brief	指定パーツの情報取得
 */
/* ================================================ */
const Common::PARTS_INFO &PlayerCombo::GetPartsInfo( const std::string name ) const
{
	auto it = m_partsMap.find( name.c_str() );
	if( it != m_partsMap.end() ){
		return (*it).second;
	}
	DEBUG_ASSERT( 0, "パーツが見つかりません\n" );
	return (*m_partsMap.begin()).second;
}



