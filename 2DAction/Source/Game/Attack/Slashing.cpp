/* ====================================================================== */
/**
 * @brief  斬撃アニメ
 *
 * @note
 *		一回再生→自殺 他のUnitとの当たり判定があるので相応のものを継承
 *		
 */
/* ====================================================================== */

#include "Slashing.h"
#include "Game/GameRegister.h"
#include "Common/Utility/CommonGameUtility.h"
#include "Game/Enemy/EnemyManager.h"
#include "System/Draw2D/SystemDraw2DResource.h"

Slashing::Slashing( const Common::OWNER_TYPE ownerType, const TYPE_SLASHING &type, const math::Vector2 &pos, const math::Vector2 &vec, uint32_t damage )
: TaskUnit( "Slashing" )
, m_ownerType( ownerType )
, m_slashingType( type )
, m_liveTime( 0 )
, m_bladeDamage( damage )
{
	std::string jsonStr = GetJsonFileStr();
	m_drawTexture.m_pTex2D = NEW Game2DBase( jsonStr.c_str() );

	const TEX_INIT_INFO &slashingInfo = TextureResourceManager::GetInstance()->GetLoadTextureInfo( jsonStr.c_str() );
	std::string setAnim = "";
	math::Vector2 vecOffset = math::Vector2();	// プレイヤーの位置にそのまま出すのではなく向きによって多少ずらす
	switch( Utility::GetDirection( vec.x, vec.y ) ){
	default:
		break;
	case InputWatcher::BUTTON_UP:
		setAnim = "up";
		vecOffset = math::Vector2( 0.0f, slashingInfo.m_sizeHeight / -3.0f );
		break;
	case InputWatcher::BUTTON_DOWN:
		setAnim = "down";
		vecOffset = math::Vector2( 0.0f, slashingInfo.m_sizeHeight / 3.0f );
		break;
	case InputWatcher::BUTTON_LEFT:
		setAnim = "left";
		vecOffset = math::Vector2( slashingInfo.m_sizeWidth / -3.0f, 0.0f );
		break;
	case InputWatcher::BUTTON_RIGHT:
		setAnim = "right";
		vecOffset = math::Vector2( slashingInfo.m_sizeWidth / 3.0f, 0.0f );
		break;
	}
	m_drawTexture.m_pTex2D->SetAnim( setAnim );
	
	//!初期位置セット
	m_drawTexture.m_texInfo.Init();
	m_drawTexture.m_texInfo.m_fileName = jsonStr;
	m_drawTexture.m_texInfo.m_posOrigin = pos + vecOffset;
	m_drawTexture.m_texInfo.m_prioity = PRIORITY_ABOVE_NORMAL;
	m_drawTexture.m_pTex2D->SetDrawInfo(m_drawTexture.m_texInfo);
}

Slashing::~Slashing(void)
{
}

bool Slashing::DieMain()
{
	return true;
}

bool Slashing::Init()
{
	return true;
}

const TEX_DRAW_INFO &Slashing::GetDrawInfo() const
{
	if( m_drawTexture.m_pTex2D == NULL ){
		DEBUG_ASSERT( 0, "弾の描画クラスがNULL");
	}
	return m_drawTexture.m_pTex2D->GetDrawInfo();
}

/* ================================================ */
/**
 * @brief	各種Update関数
 */
/* ================================================ */
void Slashing::Update()
{
	m_drawTexture.m_pTex2D->SetDrawInfo(m_drawTexture.m_texInfo);
	++m_liveTime;
}

void Slashing::DrawUpdate()
{
	if( std::string("").compare(m_drawTexture.m_pTex2D->GetPlayAnim()) == 0 ){
		// アニメは終わったので自殺、親にも伝える
		Message msg = Message( SLASHING_ANIM_END );
		SendMessageToParent( msg );
		TaskStartDie();
		return;
	}

	// 描画
	m_drawTexture.m_pTex2D->DrawUpdate2D();
}

/* ================================================ */
/**
 * @brief	他クラスからのイベント処理
 */
/* ================================================ */
void Slashing::EventUpdate( const Common::CMN_EVENT &eventId )
{
	switch( eventId.m_event ){
	default:

		break;

	case Common::EVENT_HIT_ENEMY_AAA:	// 敵に当たった
	case Common::EVENT_HIT_ENEMY_BBB:
	case Common::EVENT_HIT_ENEMY_CCC:
		if( m_ownerType == Common::OWNER_PLAYER ){
			//m_liveTime = BULLET_LIVE_TIME;
			//TaskStartDie();
		}
		break;
		
	case Common::EVENT_HIT_PLAYER:
		if( m_ownerType == Common::OWNER_ENEMY ){
			//m_liveTime = BULLET_LIVE_TIME;
			//TaskStartDie();
		}
		break;
	}
}

/* ================================================ */
/**
 * @brief	このクラスの種類セット
 */
/* ================================================ */
const Common::TYPE_OBJECT Slashing::GetTypeObject() const
{
	Common::TYPE_OBJECT type = Common::TYPE_BLADE_PLAYER;
	if( m_ownerType == Common::OWNER_ENEMY ){
		type = Common::TYPE_BLADE_ENEMY;
	}
	return type;
}

const std::string Slashing::GetJsonFileStr()
{
	std::string retStr = "";
	
	switch( m_slashingType ){
	default:
		DEBUG_ASSERT( 0, "想定外の斬撃タイプ" );
		/* fall-through */
	case TYPE_1ST:
		retStr = "slashing1st.json";
		break;
	case TYPE_2ND:
		retStr = "slashing2nd.json";
		break;
	case TYPE_3RD:
		retStr = "slashing3rd.json";
		break;
	}
	return retStr;
}
