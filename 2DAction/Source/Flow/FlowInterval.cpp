/* ====================================================================== */
/**
 * @brief  
 *		ゲーム終了後リトライ、次に進むか確認する画面
 * @note
 *		
 */
/* ====================================================================== */
#include "FlowInterval.h"
#include "Game/GameRecorder.h"

FlowBase *FlowInterval::Create( const std::string &fileName )
{
	return NEW FlowInterval(fileName);
}

FlowInterval::FlowInterval( const std::string &fileName )
: FlowBase(fileName)
, m_pRetryTex( NULL )
{
	DEBUG_PRINT("FlowInterval生成！！\n");
}


FlowInterval::~FlowInterval(void)
{
	DEBUG_PRINT("FlowInterval削除！！\n");
}

bool FlowInterval::Init()
{
	// 一枚絵作成
	m_pRetryTex = Interval2D::CreateRetry2D();
	return true;
}

/* ================================================ */
/**
 * @brief	パッド操作関数
 */
/* ================================================ */
void FlowInterval::PadEventDecide()
{
	// 決定SE鳴らす
	SoundManager::GetInstance()->PlaySE("Decide");

	std::string eventStr = GetNextFadeStr().c_str();
	if( eventStr.compare("") != 0){
		StartFade( eventStr.c_str() );
	}
}

/* ================================================ */
/**
 * @brief	次に進むべきラベル取得関数
 */
/* ================================================ */
std::string FlowInterval::GetNextFadeStr()
{
	std::string retStr = "";

	if( m_pRetryTex->GetSelectedNo() == Interval2D::SELECT_TITLE ){
		retStr = "title";
	}
	else{
		GameRecorder *pRecorder = GameRecorder::GetInstance();
		if( pRecorder ){
			switch( pRecorder->GetGameStateOfProgress() ){
			case GameRecorder::STATE_TITLE:
			case GameRecorder::STATE_POWER_UP:
			case GameRecorder::STATE_STAGE03:
			default:
				DEBUG_ASSERT( 0, "想定外のフロー" );
				// とりあえずタイトルへ
				retStr = "title";
				break;
			case GameRecorder::STATE_STAGE01:
				retStr = "nextgame02";
				break;
			case GameRecorder::STATE_STAGE02:
				retStr = "nextgame03";
				break;
			}
		}
	}
	return retStr;
}


/* ====================================================================== */
/**
 * @brief  
 *		FlowIntervalの表示物中身
 * @note
 *		
 */
/* ====================================================================== */
Interval2D *Interval2D::CreateRetry2D()
{
	return NEW Interval2D();
}

Interval2D::Interval2D()
: TaskUnit("Interval2D")
, m_selectNo( 0 )
{
	m_textureRetry.Init();

	std::string readBgJsonStr = "";
	GameRecorder *pRecorder = GameRecorder::GetInstance();
	if( pRecorder ){
		switch( pRecorder->GetGameStateOfProgress() ){
		case GameRecorder::STATE_TITLE:
		case GameRecorder::STATE_POWER_UP:
		case GameRecorder::STATE_STAGE03:
		default:
			DEBUG_ASSERT( 0, "想定外のフロー" );
			/* fall-througt */
		case GameRecorder::STATE_STAGE01:
			readBgJsonStr = "resultBgStage02.json";
			break;
		case GameRecorder::STATE_STAGE02:
			readBgJsonStr = "resultBgStage03.json";
			break;
		}
	}

	// 描画クラスセットアップ
	m_textureRetry.m_pTex2D = NEW Game2DBase( readBgJsonStr.c_str() );
	m_textureRetry.m_texInfo.m_fileName = readBgJsonStr;
	m_textureRetry.m_texInfo.m_posOrigin.x = WINDOW_WIDTH / 2.0f;
	m_textureRetry.m_texInfo.m_posOrigin.y = WINDOW_HEIGHT / 2.0f;
	m_textureRetry.m_texInfo.m_usePlayerOffset = false;
	m_textureRetry.m_pTex2D->SetDrawInfo(m_textureRetry.m_texInfo);

	m_texInfo.Init();
	m_texInfo.m_usePlayerOffset = false;
	for( uint32_t i = 0; i < SELECT_RETRY_MAX; ++i ){
		m_pTexChoiceArray[i] = NULL;
		m_pTexChoiceBGArray[i] = NULL;

		m_pTexChoiceArray[i] = NEW Game2DBase("choice.json");
		m_pTexChoiceArray[i]->SetDrawInfo( m_texInfo );
		m_pTexChoiceBGArray[i] = NEW Game2DBase("choiceBG.json");
		m_pTexChoiceBGArray[i]->SetDrawInfo( m_texInfo );
	}
}

Interval2D::~Interval2D(void)
{
	m_textureRetry.DeleteAndInit();
	
	for( uint32_t i = 0; i < SELECT_RETRY_MAX; ++i ){
		SAFE_DELETE( m_pTexChoiceArray[i] );
		SAFE_DELETE( m_pTexChoiceBGArray[i] );
	}
}


bool Interval2D::Init()
{
	// 方向キーのパッドイベントはPUSHで呼ぶように設定
	SetPadButtonState( InputWatcher::BUTTON_UP,		InputWatcher::EVENT_PUSH );
	SetPadButtonState( InputWatcher::BUTTON_DOWN,	InputWatcher::EVENT_PUSH );
	SetPadButtonState( InputWatcher::BUTTON_RIGHT,	InputWatcher::EVENT_PUSH );
	SetPadButtonState( InputWatcher::BUTTON_LEFT,	InputWatcher::EVENT_PUSH );

	return true;
}

void Interval2D::PadEventUp()
{
}

void Interval2D::PadEventDown()
{
}
void Interval2D::PadEventRight()
{
	// カーソルSE鳴らす
	SoundManager::GetInstance()->PlaySE("Cursor");
	m_selectNo = (m_selectNo+1) % SELECT_RETRY_MAX;
}
void Interval2D::PadEventLeft()
{
	// カーソルSE鳴らす
	SoundManager::GetInstance()->PlaySE("Cursor");
	m_selectNo = (m_selectNo+(SELECT_RETRY_MAX - 1)) % SELECT_RETRY_MAX;
}

void Interval2D::Update()
{
	CallPadEvent();
}

void Interval2D::DrawUpdate()
{
	if( m_textureRetry.m_pTex2D ){
		m_textureRetry.m_pTex2D->DrawUpdate2D();
	}
	
	for( uint32_t i = 0; i < SELECT_RETRY_MAX; ++i){
		if( !m_pTexChoiceArray[i] || !m_pTexChoiceBGArray[i] ){
			continue;
		}
		switch(i){
		default:
			DEBUG_ASSERT( 0, "想定外の値" );
			m_pTexChoiceArray[i]->SetAnim("title");
			m_texInfo.m_posOrigin = math::Vector2( 100.0f, 40.0f );
			break;
		case SELECT_NEXT:
			m_pTexChoiceArray[i]->SetAnim("next");
			m_texInfo.m_posOrigin = math::Vector2( 100.0f, 40.0f );
			break;
		case SELECT_TITLE:
			m_pTexChoiceArray[i]->SetAnim("title");
			m_texInfo.m_posOrigin = math::Vector2( 400.0f, 40.0f );
			break;
		}

		// カーソルが当たっていたらアニメ変更
		std::string anim = "default";
		if( m_selectNo == i ){
			anim = "spot";
		}
		m_pTexChoiceBGArray[i]->SetAnim( anim );

		// 描画位置更新
		m_pTexChoiceArray[i]->SetDrawInfo( m_texInfo );
		m_pTexChoiceBGArray[i]->SetDrawInfo( m_texInfo );

		// 描画
		m_pTexChoiceBGArray[i]->DrawUpdate2D();
		m_pTexChoiceArray[i]->DrawUpdate2D();
	}
}
