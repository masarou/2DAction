/* ====================================================================== */
/**
 * @brief  フィールドに無作為に出てくるアイテムクラス
 *
 * @note
 *		
 */
/* ====================================================================== */

#include "ItemObject.h"
#include "Game/GameRegister.h"
#include "Common/Utility/CommonGameUtility.h"
#include "System/Sound/SystemSoundManager.h"

ItemObject *ItemObject::Create( const ITEM_KIND &kind, math::Vector2 pos )
{
	return NEW ItemObject( kind, pos );
}

ItemObject::ItemObject( const ITEM_KIND &kind, math::Vector2 pos )
: TaskUnit("ItemObject")
, m_kindItem(kind)
, m_liveTime(0)
{
	m_drawTexture.m_texInfo.m_posOrigin = pos;
}


ItemObject::~ItemObject(void)
{
}

bool ItemObject::Init()
{
	m_drawTexture.m_pTex2D = NEW Game2DBase( GetItemFilePath().c_str() );

	//!初期位置セット
	m_drawTexture.m_texInfo.m_fileName = GetItemFilePath().c_str();
	if( m_drawTexture.m_texInfo.m_posOrigin == DEFAULT_VECTOR2 ){
		m_drawTexture.m_texInfo.m_posOrigin = Utility::GetMapRandamPos( /*allowInWindow=*/false );
	}
	m_drawTexture.m_pTex2D->SetDrawInfo( m_drawTexture.m_texInfo );

	return true;
}

bool ItemObject::DieMain()
{
	if( GameRegister::GetInstance()->GetManagerItem() ){
		GameRegister::GetInstance()->UpdateManagerItem()->RemoveItem( this );
	}
	return true;
}

void ItemObject::Update()
{
	// 生存時間更新
	++m_liveTime;
}

void ItemObject::DrawUpdate()
{
	// 消える三秒前ぐらいから点滅させる
	if( m_liveTime > ITEM_LIVE_TIME - 180
		&& m_liveTime < ITEM_LIVE_TIME ){
		if( m_liveTime%3 != 1 ){
			m_drawTexture.m_pTex2D->DrawUpdate2D();
		}
	}
	else if( m_liveTime >= ITEM_LIVE_TIME ){
		// 死亡
		TaskStartDie();
	}
	else{
		// アイテム描画
		m_drawTexture.m_pTex2D->DrawUpdate2D();
	}
}

/* ================================================ */
/**
 * @brief	描画情報取得関数
 */
/* ================================================ */
const TEX_DRAW_INFO &ItemObject::GetDrawInfo() const
{
	if( m_drawTexture.m_pTex2D == NULL ){
		DEBUG_ASSERT( 0, "アイテムの描画クラスがNULL");
	}
	return m_drawTexture.m_pTex2D->GetDrawInfo();
}

/* ================================================ */
/**
 * @brief	自分が何者なのかを示すタイプ取得
 */
/* ================================================ */
const Common::TYPE_OBJECT ItemObject::GetTypeObject() const
{
	Common::TYPE_OBJECT type = Common::TYPE_MAX;
	switch( m_kindItem ){
	default:
		DEBUG_ASSERT( 0, "アイテムの種類が想定外" );
		/* fall-through */
	case ITEM_KIND_RAPID_BULLET:
		type = Common::TYPE_ITEM_BULLET;
		break;
	case ITEM_KIND_LIFE_UP:
		type = Common::TYPE_ITEM_LIFE;
		break;
	case ITEM_KIND_DAMAGE_UP:
		type = Common::TYPE_ITEM_DAMAGE;
		break;
	};
	return type;
}

/* ================================================ */
/**
 * @brief	イベント処理
 */
/* ================================================ */
void ItemObject::EventUpdate( const Common::CMN_EVENT &eventId )
{
	switch( eventId.m_event ){
	case Common::EVENT_HIT_PLAYER:
		m_liveTime = ITEM_LIVE_TIME;
		TaskStartDie();
		SoundManager::GetInstance()->PlaySE("ShootBullet");	// 取得音を鳴らす
		break;
	default:
		break;
	}
}

/* ================================================ */
/**
 * @brief	アイテムの種類から対応するjsonファイル名を取得
 */
/* ================================================ */
std::string ItemObject::GetItemFilePath()
{
	std::string fileName = "";
	switch( m_kindItem ){
	default:
	case ITEM_KIND_RAPID_BULLET:
		fileName = "itemBullet.json";
		break;
	case ITEM_KIND_LIFE_UP:
		fileName = "itemLife.json";
		break;
	case ITEM_KIND_DAMAGE_UP:
		fileName = "itemDamage.json";
		break;
	};

	return fileName;
}