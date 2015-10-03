/* ====================================================================== */
/**
 * @brief  
 *		ゲーム履歴表示クラス
 * @note
 *		
 */
/* ====================================================================== */

#include "FlowViewScore.h"
#include "Common/Utility/CommonGameUtility.h"

FlowBase *FlowViewScore::Create( const std::string &fileName )
{
	return NEW FlowViewScore(fileName);
}

FlowViewScore::FlowViewScore( const std::string &fileName )
: FlowMenuBase(fileName)
{
	DEBUG_PRINT("FlowViewScore生成！！\n");
}


FlowViewScore::~FlowViewScore(void)
{
	DEBUG_PRINT("FlowResult削除！！\n");
}

bool FlowViewScore::Init()
{
	// 背景一枚絵作成
	m_pMenuWindow = ViewScoreMenu::CreateViewScore2D( "MenuViewScore.json" );
	return true;
}

/* ====================================================================== */
/**
 * @brief  
 *		表示する数字カウンタ
 * @note
 *		
 */
/* ====================================================================== */
ViewScoreMenu *ViewScoreMenu::CreateViewScore2D( const std::string &readMenuJson )
{
	return NEW ViewScoreMenu( readMenuJson );
}

ViewScoreMenu::ViewScoreMenu( const std::string &readMenuJson )
: MenuWindow( readMenuJson )
{
}

ViewScoreMenu::~ViewScoreMenu(void)
{
}

bool ViewScoreMenu::InitMenu()
{
	// キャンセルボタン有効に
	SetPadButtonState( InputWatcher::BUTTON_DOWN,	InputWatcher::EVENT_PUSH );

	// ランキング取得
	Utility::GetSaveData( m_saveData );

	// ランキング描画
	for( uint32_t i = 0; i < Common::RANKING_RECORD_MAX; ++i ){
		std::string partsStr = "rank";
		partsStr += '0' + i;
		PartsCounter *pCounter = GetPartsCounter( partsStr );
		if( pCounter ){
			pCounter->AddValue( m_saveData.m_scoreRanking[i] );
		}
	}

	return true;
}


void ViewScoreMenu::UpdateMenu()
{
	if( !GetNextFlowStr().empty() ){
		// 次の遷移先が決まったのでなにもしない
		return;
	}

	CallPadEvent();
}

/* ================================================ */
/**
 * @brief	パッド操作関数
 */
/* ================================================ */
void ViewScoreMenu::PadEventCancel()
{
	// キャンセルSE鳴らす
	SoundManager::GetInstance()->PlaySE("Cancel");
	SetNextFlowStr( "return" );
}