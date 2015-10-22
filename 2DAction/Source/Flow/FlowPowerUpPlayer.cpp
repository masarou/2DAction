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
#include "Flow/Process/FlowProcessFirstManual.h"

#include "Common/Utility/CommonGameUtility.h"

#define KONAMI_COMMAND_SUCCESS 573

FlowBase *FlowPowerUpPlayer::Create( const std::string &fileName )
{
	return NEW FlowPowerUpPlayer(fileName);
}

FlowPowerUpPlayer::FlowPowerUpPlayer( const std::string &fileName )
: FlowMenuBase(fileName)
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

	Common::SAVE_DATA saveData;
	Utility::GetSaveData( saveData );

	if( saveData.m_isFirst ){
		// 最初の説明
		FirstManual *pFirstManual = FirstManual::Create( FirstManual::KIND_POWERUP1 );
		PushStageEffect( pFirstManual );
		pFirstManual = FirstManual::Create( FirstManual::KIND_POWERUP2 );
		PushStageEffect( pFirstManual );
		pFirstManual = FirstManual::Create( FirstManual::KIND_POWERUP3 );
		PushStageEffect( pFirstManual );
		pFirstManual = FirstManual::Create( FirstManual::KIND_POWERUP4 );
		PushStageEffect( pFirstManual );
		pFirstManual = FirstManual::Create( FirstManual::KIND_POWERUP5 );
		PushStageEffect( pFirstManual );
		pFirstManual = FirstManual::Create( FirstManual::KIND_POWERUP6 );
		PushStageEffect( pFirstManual );
	}

	return true;
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
, m_isProceedEnd( false )
, m_selectStateKind( Common::BASE_STATE_LIFE )
, m_explanationStr("")
{
	// セーブデータロード
	Utility::GetSaveData( m_loadData );
}

PowerUpMenu::~PowerUpMenu(void)
{

}

/* ================================================ */
/**
 * @brief	画面準備
 */
/* ================================================ */
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
		drawInfo.m_prioity = Common::PRIORITY_LOW;
	}

	pParts = GetParts( "decisionUp" );
	if( pParts ){
		TEX_DRAW_INFO &drawInfo = pParts->GetTexDrawInfo();
		drawInfo.m_prioity = Common::PRIORITY_LOW;
	}

	// 画面描画更新
	ChangeDispState( m_selectStateKind );
	UpdateCursorMove();

	{
		CommandTable *table = NEW CommandTable(20, KONAMI_COMMAND_SUCCESS);
		table->m_table[0][0] = BUTTON_UP;
		table->m_table[1][0] = BUTTON_UP;
		table->m_table[2][0] = BUTTON_DOWN;
		table->m_table[3][0] = BUTTON_DOWN;
		table->m_table[4][0] = BUTTON_LEFT;
		table->m_table[5][0] = BUTTON_RIGHT;
		table->m_table[6][0] = BUTTON_LEFT;
		table->m_table[7][0] = BUTTON_RIGHT;
		table->m_table[8][0] = BUTTON_CANCEL;
		table->m_table[9][0] = BUTTON_DECIDE;
		MakeCommandTable(table);
	}

	return true;
}

/* ================================================ */
/**
 * @brief	死亡前処理
 */
/* ================================================ */
bool PowerUpMenu::DieMainMenu()
{
	if( m_isProceedEnd ){
		// 余ったポイントはなくす
		m_loadData.m_battlePoint = 0;
	}

	// 解説終了
	m_loadData.m_isFirst = false;

	// ここで行ったパワーアップをセーブデータに反映
	Utility::OverWriteSaveData( m_loadData );
	return true;
}

/* ================================================ */
/**
 * @brief	描画する強化項目説明更新
 */
/* ================================================ */
void PowerUpMenu::ChangeDispState( const Common::PLAYER_BASE_STATE &kind )
{
	std::string	setAnimStr		= "";	// セットする画像アニメ文字列
	uint32_t	currentLv		= m_loadData.m_playerBaseStateLv[kind];	// 現在のLv
	uint32_t	pointToNextLv	= GetPointToNextLevel( kind, currentLv );	// 次のLvまでの必要ポイント
	
	switch( kind ){
	case Common::BASE_STATE_LIFE:		// ライフの最大値を決める
		setAnimStr = "life";
		break;

	case Common::BASE_STATE_MOVE_SPEED:	// ダッシュ(移動速度)時間
		setAnimStr = "movespd";
		break;

	case Common::BASE_STATE_DEFFENCE:	// 被ダメージを決める
		setAnimStr = "defence";
		break;

	case Common::BASE_STATE_BULLET_SPD:	// マシンガンの間隔
		setAnimStr = "bulletspd";
		break;

	case Common::BASE_STATE_BULLET_DMG:	// マシンガンのダメージ
		setAnimStr = "bulletdamage";
		break;
	
	case Common::BASE_STATE_BLADE_LEVEL:// 斬撃のダメージ
		setAnimStr = "bladelv";
		break;
	
	case Common::BASE_STATE_CONTINUE:	// コンティニュー
		setAnimStr = "continue";
		break;
	}

	// 項目画像表示
	SetAnim( "itemImage", setAnimStr );

	// 表示箇所の描画
	for( uint32_t i = 0; i < Common::BASE_STATE_MAX ; ++i ){
		std::string animStr = "idle";
		if( i == static_cast<uint32_t>(kind) ){
			animStr = "select";
		}
		std::string pointPartsStr = "point";
		pointPartsStr += '0' + i;
		SetAnim( pointPartsStr, animStr );
	}

	// 現在レベルセット
	PartsCounter *pPartsCurrLevel = GetPartsCounter("currLevel");
	if( pPartsCurrLevel ){
		pPartsCurrLevel->SetValue( currentLv+1 );
		pPartsCurrLevel->SetCountAnimFlg( false );
	}

	// 最大レベルセット
	PartsCounter *pPartsMaxLevel = GetPartsCounter("levelMax");
	if( pPartsMaxLevel ){
		if( kind == Common::BASE_STATE_CONTINUE ){
			pPartsMaxLevel->SetValue( 2 );
		}
		else{
			pPartsMaxLevel->SetValue( 10 );
		}
		pPartsMaxLevel->SetCountAnimFlg( false );
	}

	// 次のLvまでのポイントセット
	PartsCounter *pPartsPointToNext = GetPartsCounter("nextPoint");
	if( pPartsPointToNext ){
		if( pointToNextLv == INVALID_VALUE ){
			pPartsPointToNext->SetValue( 9999 );
		}
		else{
			pPartsPointToNext->SetValue( pointToNextLv );
		}
		pPartsPointToNext->SetCountAnimFlg( false );
	}

	// 所持ポイントセット
	PartsCounter *pPartsCurrPoint = GetPartsCounter("currPoint");
	if( pPartsCurrPoint ){
		pPartsCurrPoint->SetValue( m_loadData.m_battlePoint );
		pPartsCurrPoint->SetCountAnimFlg( false );
	}

	// レベルアップ可能かどうか
	MenuParts *pPartsDecision = GetParts( "decisionUp" );
	if( pPartsDecision ){
		if( pointToNextLv <= m_loadData.m_battlePoint ){
			// LvUp可能
			SetAnim( "decisionUp", "possible" );
		}
		else{
			SetAnim( "decisionUp", "inpossible" );
		}
	}

	// 説明文更新
	m_explanationStr = GetExplanationStr( kind );
}

/* ================================================ */
/**
 * @brief	カーソル移動の処理まとめ
 */
/* ================================================ */
void PowerUpMenu::UpdateCursorMove()
{
	if( GetSelectedNo() == SEELCT_ITEM ){
		SetAnim( "frame", "spot" );
	}
	else{
		SetAnim( "frame", "idle" );
	}
}

void PowerUpMenu::UpdateMenu()
{
	CallPadEvent();
	
	// 説明文セット
	Draw2DManager::GetInstance()->PushDrawString( m_explanationStr, math::Vector2( 300.0f, 430.0f ) );
}

/* ================================================ */
/**
 * @brief	パッド操作関数
 */
/* ================================================ */
void PowerUpMenu::PadEventDecide()
{
	if( GetSelectedNo() == SELECT_GAME_START ){
		// 決定SE鳴らす
		SoundManager::GetInstance()->PlaySE("Decide");

		// コンティニュー出来るかどうかチェック
		if( m_loadData.m_playerBaseStateLv[Common::BASE_STATE_CONTINUE] == 0 ){
			// 直接ゲーム画面へ
			SetNextFlowStr( "proceed" );
		}
		else{
			// ステージセレクトへ
			SetNextFlowStr( "stageselect" );
		}

		// 決定進行フラグを立てる = PPを0
		m_isProceedEnd = true;
	}
	else{

		uint32_t needPoint = GetPointToNextLevel( m_selectStateKind, m_loadData.m_playerBaseStateLv[m_selectStateKind] );
		if( m_loadData.m_battlePoint < needPoint
			|| needPoint == INVALID_VALUE )
		{
			// ポイントが足りない or Lvが最大
			// エラー音
			SoundManager::GetInstance()->PlaySE("Error");
			return;
		}

		// LvUp効果音
		SoundManager::GetInstance()->PlaySE("LearningAction");

		// 必要ポイント分を差っぴく
		m_loadData.m_battlePoint -= needPoint;
		
		// 項目のパワーアップ
		m_loadData.m_playerBaseStateLv[m_selectStateKind] += 1;

		// 表示更新
		ChangeDispState( m_selectStateKind );
	}
}

void PowerUpMenu::PadEventUp()
{
	// カーソルSE鳴らす
	SoundManager::GetInstance()->PlaySE("Cursor");

	uint32_t selectNo = ( GetSelectedNo() + 1 ) % SELECT_MAX;
	SetSelectNum( selectNo );

	// 描画更新
	UpdateCursorMove();
}

void PowerUpMenu::PadEventDown()
{
	// カーソルSE鳴らす
	SoundManager::GetInstance()->PlaySE("Cursor");

	uint32_t selectNo = ( GetSelectedNo() + (SELECT_MAX - 1) ) % SELECT_MAX;
	SetSelectNum( selectNo );

	// 描画更新
	UpdateCursorMove();
}

void PowerUpMenu::PadEventRight()
{
	if( GetSelectedNo() != SEELCT_ITEM ){
		// カーソルがGameStartにあるので何もしない
		return;
	}

	// カーソルSE鳴らす
	SoundManager::GetInstance()->PlaySE("Cursor");

	uint32_t selectNo = ( static_cast<uint32_t>(m_selectStateKind) + 1 ) % Common::BASE_STATE_MAX;
	m_selectStateKind = static_cast<Common::PLAYER_BASE_STATE>(selectNo);
	ChangeDispState( m_selectStateKind );
}

void PowerUpMenu::PadEventLeft()
{
	if( GetSelectedNo() != SEELCT_ITEM ){
		// カーソルがGameStartにあるので何もしない
		return;
	}

	// カーソルSE鳴らす
	SoundManager::GetInstance()->PlaySE("Cursor");

	uint32_t selectNo = ( static_cast<uint32_t>(m_selectStateKind) + (Common::BASE_STATE_MAX - 1) ) % Common::BASE_STATE_MAX;
	m_selectStateKind = static_cast<Common::PLAYER_BASE_STATE>(selectNo);
	ChangeDispState( m_selectStateKind );
}

void PowerUpMenu::PadEventCommand( const uint32_t &commandKind )
{
	if( commandKind == KONAMI_COMMAND_SUCCESS ){
		// カーソルSE鳴らす
		SoundManager::GetInstance()->PlaySE("konamiCommand");
		// 所持ポイントを最大に
		m_loadData.m_battlePoint = 9998;
		// 表示更新
		ChangeDispState( m_selectStateKind );
	}
}

/* ================================================ */
/**
 * @brief	項目ごとの説明文取得
 */
/* ================================================ */
std::string PowerUpMenu::GetExplanationStr( const Common::PLAYER_BASE_STATE &kind )
{
	std::string retStr = "";
	switch( kind ){
	case Common::BASE_STATE_LIFE:		// ライフの最大値を決める
		retStr = "ゲーム中のライフの最大値が増えます。";
		break;

	case Common::BASE_STATE_MOVE_SPEED:	// ダッシュ(移動速度)時間
		retStr = "L1もしくはR1を押すことでダッシュを使用することができる\nようになります。";
		retStr += "Levelが上がるとダッシュ時間が増加します。";
		break;

	case Common::BASE_STATE_DEFFENCE:	// 被ダメージを決める
		retStr = "敵から受けるダメージを軽減します。";
		break;

	case Common::BASE_STATE_BULLET_SPD:	// マシンガンの間隔
		retStr = "右スティックで発射する弾丸の発射感覚が短くなり、\nより多くの弾が発射されるようになります。";
		break;

	case Common::BASE_STATE_BULLET_DMG:	// マシンガンのダメージ
		retStr = "右スティックで発射する弾丸の威力が増加します。";
		break;
	
	case Common::BASE_STATE_BLADE_LEVEL:	// 斬撃のダメージ
		retStr = "決定キーで出せる斬撃の威力が増加します。";
		break;
	
	case Common::BASE_STATE_CONTINUE:		// コンティニュー
		retStr = "ゲームを開始するときに以前クリアしたStageから始める\nことができるようになります。";
		break;
	}
	return retStr;
}


/* ================================================ */
/**
 * @brief	次のレベルまでのポイントを取得
 */
/* ================================================ */
uint32_t PowerUpMenu::GetPointToNextLevel( const Common::PLAYER_BASE_STATE &kind, uint32_t currLevel )
{
	
	static const uint32_t levelPointTableLow[] = {
		10,20,30,40,50,60,70,80,100
	};
	static const uint32_t levelPointTableMiddle[] = {
		10,20,30,40,50,60,80,100,120
	};
	static const uint32_t levelPointTableHigh[] = {
		10,20,30,50,70,90,110,140,170,200
	};

	uint32_t retVal = 0;

	switch( kind ){
	case Common::BASE_STATE_LIFE:				// ライフの最大値を決める
		retVal = levelPointTableLow[currLevel];
		break;

	case Common::BASE_STATE_MOVE_SPEED:			// ダッシュ(移動速度)時間
		retVal = levelPointTableLow[currLevel];
		break;

	case Common::BASE_STATE_DEFFENCE:			// 被ダメージを決める
		retVal = levelPointTableMiddle[currLevel];
		break;

	case Common::BASE_STATE_BULLET_SPD:			// マシンガンの間隔
		retVal = levelPointTableMiddle[currLevel];
		break;

	case Common::BASE_STATE_BULLET_DMG:			// マシンガンのダメージ
		retVal = levelPointTableHigh[currLevel];
		break;
	
	case Common::BASE_STATE_BLADE_LEVEL:		// 斬撃のダメージ
		retVal = levelPointTableHigh[currLevel];
		break;

	case Common::BASE_STATE_CONTINUE:		// コンティニュー
		retVal = ( currLevel == 0 ) ? 300 : 9999 ;
		break;
	}

	if( currLevel >= Common::STATUS_LEVEL_MAX ){
		// 既に最大
		return INVALID_VALUE;
	}

	return retVal;
}