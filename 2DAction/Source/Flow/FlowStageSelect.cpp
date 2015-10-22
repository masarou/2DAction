/* ====================================================================== */
/**
 * @brief  
 *		ゲーム開始前、ユーザーの基本能力UP画面
 * @note
 *		
 */
/* ====================================================================== */
#include "FlowStageSelect.h"
#include "Game/GameRecorder.h"
#include "Flow/Process/FlowProcessFirstManual.h"

#include "Common/Utility/CommonGameUtility.h"

FlowBase *FlowStageSelect::Create( const std::string &fileName )
{
	return NEW FlowStageSelect(fileName);
}

FlowStageSelect::FlowStageSelect( const std::string &fileName )
: FlowMenuBase(fileName)
{
	DEBUG_PRINT("FlowStageSelect生成！！\n");
}


FlowStageSelect::~FlowStageSelect(void)
{
	DEBUG_PRINT("FlowStageSelect削除！！\n");
}

bool FlowStageSelect::Init()
{
	m_pMenuWindow = StageSelectMenu::CreateStageSelectMenu( "MenuStageSelect.json" );
	return true;
}

/* ====================================================================== */
/**
 * @brief  
 *		FlowStageSelectの表示物中身
 * @note
 *		
 */
/* ====================================================================== */
StageSelectMenu *StageSelectMenu::CreateStageSelectMenu( const std::string &fileName )
{
	return NEW StageSelectMenu( fileName );
}

StageSelectMenu::StageSelectMenu( const std::string &fileName )
: MenuWindow( fileName )
, m_selectStageKind( SEELCT_STAGE01 )
, m_selectStageMax( SEELCT_STAGE02 )
{
	Common::SAVE_DATA saveData;
	Utility::GetSaveData( saveData );
	if( saveData.m_isClearPhase02 ){
		m_selectStageMax = SELECT_MAX;
	}
	else if( saveData.m_isClearPhase01 ){
		m_selectStageMax = SEELCT_STAGE03;
	}
}

StageSelectMenu::~StageSelectMenu(void)
{

}

/* ================================================ */
/**
 * @brief	画面準備
 */
/* ================================================ */
bool StageSelectMenu::InitMenu()
{
	// 方向キーのパッドイベントはPUSHで呼ぶように設定
	SetPadButtonState( InputWatcher::BUTTON_RIGHT,	InputWatcher::EVENT_PUSH );
	SetPadButtonState( InputWatcher::BUTTON_LEFT,	InputWatcher::EVENT_PUSH );

	// パーツの準備
	MenuParts *pParts = GetParts( "frame" );
	if( pParts ){
		TEX_DRAW_INFO &drawInfo = pParts->GetTexDrawInfo();
		drawInfo.m_prioity = Common::PRIORITY_ABOVE_NORMAL;
	}
	pParts = GetParts( "stage01" );
	if( pParts ){
		TEX_DRAW_INFO &drawInfo = pParts->GetTexDrawInfo();
		drawInfo.m_prioity = Common::PRIORITY_BELOW_NORMAL;
	}
	pParts = GetParts( "stage02" );
	if( pParts ){
		TEX_DRAW_INFO &drawInfo = pParts->GetTexDrawInfo();
		drawInfo.m_prioity = Common::PRIORITY_BELOW_NORMAL;
	}
	pParts = GetParts( "stage03" );
	if( pParts ){
		TEX_DRAW_INFO &drawInfo = pParts->GetTexDrawInfo();
		drawInfo.m_prioity = Common::PRIORITY_BELOW_NORMAL;
	}

	// 画面描画更新
	ChangeDispState( m_selectStageKind );

	return true;
}

/* ================================================ */
/**
 * @brief	死亡前処理
 */
/* ================================================ */
bool StageSelectMenu::DieMainMenu()
{
	return true;
}

/* ================================================ */
/**
 * @brief	描画する強化項目説明更新
 */
/* ================================================ */
void StageSelectMenu::ChangeDispState( const SELECT_STAGE &stage )
{
	MenuParts *pParts = NULL;

	std::string partsName = "stage0";

	// いったん背景を全非表示
	for( uint32_t i = 0; i < SELECT_MAX ; ++i ){
		std::string partsStr = partsName;
		partsStr += '1' + i;
		pParts = GetParts( partsStr.c_str() );
		if( pParts ){
			pParts->SetDrawFlag( false );
		}
	}

	std::string dispPartsStr = partsName;
	dispPartsStr += '1' + static_cast<uint32_t>(stage);
	pParts = GetParts( dispPartsStr.c_str() );
	if( pParts ){
		pParts->SetDrawFlag( true );
	}

	// 説明文更新
	m_explanationStr = GetExplanationStr( stage );
}

void StageSelectMenu::UpdateMenu()
{
	CallPadEvent();
	
	// 説明文セット
	Draw2DManager::GetInstance()->PushDrawString( m_explanationStr, math::Vector2( 270.0f, 480.0f ) );
}

/* ================================================ */
/**
 * @brief	パッド操作関数
 */
/* ================================================ */
void StageSelectMenu::PadEventDecide()
{
	// 決定SE鳴らす
	SoundManager::GetInstance()->PlaySE("Decide");

	switch( m_selectStageKind ){
	default:
		DEBUG_ASSERT( 0, "想定外の値");
	case SEELCT_STAGE01:
		SetNextFlowStr( "start01" );
		break;
	case SEELCT_STAGE02:
		SetNextFlowStr( "start02" );
		// 途中からプレイかどうかフラグセット(ランキング反映なし)
		GameRecorder::GetInstance()->SetContinueFlag();
		break;
	case SEELCT_STAGE03:
		// 途中からプレイかどうかフラグセット(ランキング反映なし)
		GameRecorder::GetInstance()->SetContinueFlag();
		SetNextFlowStr( "start03" );
		break;
	}
}

void StageSelectMenu::PadEventRight()
{
	// カーソルSE鳴らす
	SoundManager::GetInstance()->PlaySE("Cursor");

	uint32_t selectNo = ( static_cast<uint32_t>(m_selectStageKind) + 1 ) % m_selectStageMax;
	m_selectStageKind = static_cast<SELECT_STAGE>(selectNo);
	ChangeDispState( m_selectStageKind );
}

void StageSelectMenu::PadEventLeft()
{
	// カーソルSE鳴らす
	SoundManager::GetInstance()->PlaySE("Cursor");

	uint32_t selectNo = ( static_cast<uint32_t>(m_selectStageKind) + (m_selectStageMax - 1) ) % m_selectStageMax;
	m_selectStageKind = static_cast<SELECT_STAGE>(selectNo);
	ChangeDispState( m_selectStageKind );
}

/* ================================================ */
/**
 * @brief	項目ごとの説明文取得
 */
/* ================================================ */
std::string StageSelectMenu::GetExplanationStr( const SELECT_STAGE &stage )
{
	std::string retStr = "";
	switch( stage ){
	case SEELCT_STAGE01:
		retStr = "Stage1-1から始めます。\n通常のプレイ";
		break;

	case SEELCT_STAGE02:
		retStr = "Stage2-1から始めます。";
		break;

	case SEELCT_STAGE03:
		retStr = "Stage3-1から始めます。";
		break;
	}
	return retStr;
}
