/* ====================================================================== */
/**
 * @brief  
 *		ゲーム開始前、ユーザーの基本能力UP画面
 * @note
 *		
 */
/* ====================================================================== */
#include "FlowPowerUpPlayer.h"
#include "Game/GameRecorder.h"

#include "Common/Utility/CommonGameUtility.h"

FlowBase *FlowPowerUpPlayer::Create( const std::string &fileName )
{
	return NEW FlowPowerUpPlayer(fileName);
}

FlowPowerUpPlayer::FlowPowerUpPlayer( const std::string &fileName )
: FlowBase(fileName)
{
	DEBUG_PRINT("FlowPowerUpPlayer生成！！\n");
}


FlowPowerUpPlayer::~FlowPowerUpPlayer(void)
{
	DEBUG_PRINT("FlowPowerUpPlayer削除！！\n");
}

bool FlowPowerUpPlayer::Init()
{
	m_pMenuWindow = PowerUpMenu::CreatePowerUp2D( "MenuPowerUpPlayer.json" );
	return true;
}


void FlowPowerUpPlayer::UpdateFlowPreChildTask()
{
	CallPadEvent();
}

/* ================================================ */
/**
 * @brief	パッド操作関数
 */
/* ================================================ */
void FlowPowerUpPlayer::PadEventDecide()
{
	// 決定SE鳴らす
	SoundManager::GetInstance()->PlaySE("Decide");
	StartFade( "proceed" );
}

/* ====================================================================== */
/**
 * @brief  
 *		FlowPowerUpPlayerの表示物中身
 * @note
 *		
 */
/* ====================================================================== */
PowerUpMenu *PowerUpMenu::CreatePowerUp2D( const std::string &fileName )
{
	return NEW PowerUpMenu( fileName );
}

PowerUpMenu::PowerUpMenu( const std::string &fileName )
: MenuWindow( fileName )
{

}

PowerUpMenu::~PowerUpMenu(void)
{

}


// 描画する強化項目説明更新
void PowerUpMenu::ChangeDispState( const Common::PLAYER_BASE_STATE &kind )
{
	// 項目画像表示
	// auto texInfo = m_partsMap.find( "itemTex" );

	// 現在レベルセット

	// 次のLvまでのポイントセット

	// 説明文セット


	// 所持ポイントセット


}

bool PowerUpMenu::InitMenu()
{
	// 方向キーのパッドイベントはPUSHで呼ぶように設定
	SetPadButtonState( InputWatcher::BUTTON_UP,		InputWatcher::EVENT_PUSH );
	SetPadButtonState( InputWatcher::BUTTON_DOWN,	InputWatcher::EVENT_PUSH );
	SetPadButtonState( InputWatcher::BUTTON_RIGHT,	InputWatcher::EVENT_PUSH );
	SetPadButtonState( InputWatcher::BUTTON_LEFT,	InputWatcher::EVENT_PUSH );

	// パーツの準備
	MenuParts *pParts = GetParts( "frame" );
	if( pParts ){
		// 外枠は表示優先度を低くする
		TEX_DRAW_INFO &drawInfo = pParts->GetTexDrawInfo();
		drawInfo.m_prioity = PRIORITY_LOW;
	}

	static std::string s_partsStr[] = {
		"levelMax",		// 最大レベル数値
		"currLevel",	// 現在レベル数値
		"nextPoint",	// 次のレベルへのポイント
		"currPoint",	// 現在の所持ポイント
	};
	for( uint32_t i = 0; i < NUMBEROF(s_partsStr) ; ++i ){
		PartsCounter *pCounter = GetPartsCounter( s_partsStr[i] );
		if( pCounter ){
			pCounter->AddValue( 99 );
		}
	}

	return true;
}

void PowerUpMenu::UpdateMenu()
{
	
}
