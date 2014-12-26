/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		タイトル画面管理クラス
 */
/* ====================================================================== */

#include "FlowTitle.h"
#include "System/Sound/SystemSoundManager.h"

FlowBase *FlowTitle::Create( const std::string &fileName )
{
	return NEW FlowTitle(fileName);
}

FlowTitle::FlowTitle( const std::string &fileName )
: FlowBase( fileName )
, m_pTitleTex( NULL )
{
	DEBUG_PRINT("FlowTitle生成！！\n");
}


FlowTitle::~FlowTitle(void)
{
	DEBUG_PRINT("FlowTitle削除！！\n");
}

bool FlowTitle::Finish()
{
	return true;
}

bool FlowTitle::Init()
{
	// 一枚絵作成
	m_pTitleTex = Title2D::CreateTitle2D();
	return true;
}

void FlowTitle::PadEventDecide()
{
	// 決定SE鳴らす
	SoundManager::GetInstance()->PlaySE("Decide");

	switch( m_pTitleTex->GetSelectedNo() ){
	default:
		
		break;
	case Title2D::SELECT_START:
	StartFade("startgame");
		break;
	case Title2D::SELECT_SCORE:
	StartFade("score");
		break;
	case Title2D::SELECT_EXIT:
	StartFade("exit");
		break;
	}
}


/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		タイトル一枚絵クラス
 */
/* ====================================================================== */
Title2D *Title2D::CreateTitle2D()
{
	return NEW Title2D();
}

Title2D::Title2D()
: TaskUnit("Title2D")
, m_selectNo( 0 )
{
	m_textureTitle.Init();

	// 描画クラスセットアップ
	m_textureTitle.m_pTex2D = NEW Game2DBase("title.json");
	m_textureTitle.m_texInfo.Init();
	m_textureTitle.m_texInfo.m_posOrigin.x = WINDOW_WIDTH / 2.0f;
	m_textureTitle.m_texInfo.m_posOrigin.y = WINDOW_HEIGHT / 2.0f;
	m_textureTitle.m_texInfo.m_usePlayerOffset = false;
	m_textureTitle.m_pTex2D->SetDrawInfo(m_textureTitle.m_texInfo);

	m_texInfo.Init();
	m_texInfo.m_usePlayerOffset = false;
	for( uint32_t i = 0; i < SELECT_MAX; ++i ){
		m_pTexChoiceArray[i] = NULL;
		m_pTexChoiceBGArray[i] = NULL;

		m_pTexChoiceArray[i] = NEW Game2DBase("choice.json");
		m_pTexChoiceArray[i]->SetDrawInfo( m_texInfo );
		m_pTexChoiceBGArray[i] = NEW Game2DBase("choiceBG.json");
		m_pTexChoiceBGArray[i]->SetDrawInfo( m_texInfo );
	}
}

Title2D::~Title2D(void)
{
	m_textureTitle.DeleteAndInit();
	
	for( uint32_t i = 0; i < SELECT_MAX; ++i ){
		SAFE_DELETE( m_pTexChoiceArray[i] );
		SAFE_DELETE( m_pTexChoiceBGArray[i] );
	}
}


bool Title2D::Init()
{
	// 方向キーのパッドイベントはPUSHで呼ぶように設定
	SetPadButtonState( InputWatcher::BUTTON_UP,		InputWatcher::EVENT_PUSH );
	SetPadButtonState( InputWatcher::BUTTON_DOWN,	InputWatcher::EVENT_PUSH );
	SetPadButtonState( InputWatcher::BUTTON_RIGHT,	InputWatcher::EVENT_PUSH );
	SetPadButtonState( InputWatcher::BUTTON_LEFT,	InputWatcher::EVENT_PUSH );

	return true;
}

void Title2D::PadEventUp()
{
}

void Title2D::PadEventDown()
{
}
void Title2D::PadEventRight()
{
	// カーソルSE鳴らす
	SoundManager::GetInstance()->PlaySE("Cursor");
	m_selectNo = (m_selectNo+1) % SELECT_MAX;
}
void Title2D::PadEventLeft()
{
	// カーソルSE鳴らす
	SoundManager::GetInstance()->PlaySE("Cursor");
	m_selectNo = (m_selectNo+(SELECT_MAX - 1)) % SELECT_MAX;
}

void Title2D::Update()
{
	CallPadEvent();
}

void Title2D::DrawUpdate()
{
	if( m_textureTitle.m_pTex2D ){
		m_textureTitle.m_pTex2D->DrawUpdate2D();
	}
	
	for( uint32_t i = 0; i < SELECT_MAX; ++i){
		if( !m_pTexChoiceArray[i] || !m_pTexChoiceBGArray[i] ){
			continue;
		}
		switch(i){
		default:
			DEBUG_ASSERT( 0, "想定外の値" );
			m_pTexChoiceArray[i]->SetAnim("start");
			m_texInfo.m_posOrigin = math::Vector2( 100.0f, 40.0f );
			break;
		case SELECT_START:
			m_pTexChoiceArray[i]->SetAnim("start");
			m_texInfo.m_posOrigin = math::Vector2( 100.0f, 40.0f );
			break;
		case SELECT_SCORE:
			m_pTexChoiceArray[i]->SetAnim("score");
			m_texInfo.m_posOrigin = math::Vector2( 400.0f, 40.0f );
			break;
		case SELECT_EXIT:
			m_pTexChoiceArray[i]->SetAnim("exit");
			m_texInfo.m_posOrigin = math::Vector2( 700.0f, 40.0f );
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