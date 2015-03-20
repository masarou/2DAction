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

Slashing::Slashing( const Common::OWNER_TYPE ownerType, const math::Vector2 &pos, const math::Vector2 &vec, uint32_t damage )
: TaskUnit( "Slashing" )
, m_ownerType( ownerType )
, m_liveTime( 0 )
, m_bladeDamage( damage )
{
	std::string jsonStr = ( m_ownerType == Common::OWNER_PLAYER ) ? "slashing.json" : "slashing.json" ;
	m_drawTexture.m_pTex2D = NEW Game2DBase( jsonStr.c_str() );

	//!初期位置セット
	m_drawTexture.m_texInfo.Init();
	m_drawTexture.m_texInfo.m_fileName = jsonStr;
	m_drawTexture.m_texInfo.m_posOrigin = pos;
	m_drawTexture.m_texInfo.m_prioity = PRIORITY_ABOVE_NORMAL;
	m_drawTexture.m_pTex2D->SetDrawInfo(m_drawTexture.m_texInfo);

	std::string setAnim = "";
	switch( Utility::GetDirection( vec.x, vec.y ) ){
	default:
		break;
	case InputWatcher::BUTTON_UP:
		setAnim = "up";
		break;
	case InputWatcher::BUTTON_DOWN:
		setAnim = "down";
		break;
	case InputWatcher::BUTTON_LEFT:
		setAnim = "left";
		break;
	case InputWatcher::BUTTON_RIGHT:
		setAnim = "right";
		break;
	}
	m_drawTexture.m_pTex2D->SetAnim( setAnim );
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

	// 弾描画
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
	Common::TYPE_OBJECT type = Common::TYPE_BULLET_PLAYER;
	if( m_ownerType == Common::OWNER_ENEMY ){
		type = Common::TYPE_BULLET_ENEMY;
	}
	return type;
}
