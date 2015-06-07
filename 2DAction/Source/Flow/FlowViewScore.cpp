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
: FlowBase(fileName)
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

void FlowViewScore::UpdateFlowAfterChildTask()
{
	// 次の遷移先を常に監視
	if( m_pMenuWindow && !m_pMenuWindow->GetNextFlowStr().empty() ){
		StartFade( m_pMenuWindow->GetNextFlowStr().c_str() );
	}
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
			TEX_DRAW_INFO &aaa = pCounter->GetTexDrawInfo();
			aaa.m_prioity = PRIORITY_HIGHEST;
			pCounter->AddValue( m_saveData.m_scoreRanking[i] );
		}
	}

	return true;
}


void ViewScoreMenu::UpdateMenu()
{
	if( !m_nextFlow.empty() ){
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
	m_nextFlow = "return";
}