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

ItemObject *ItemObject::Create( const Common::ITEM_KIND &kind, math::Vector2 pos )
{
	return NEW ItemObject( kind, pos );
}

ItemObject::ItemObject( const Common::ITEM_KIND &kind, math::Vector2 pos )
: TaskUnit("ItemObject")
, m_kindItem(kind)
, m_liveTime(0)
{
	m_drawTexture.m_pTex2D = Game2DBase::Create( GetItemFilePath().c_str() );
	m_drawTexture.m_pTex2D->UpdateDrawInfo().m_posOrigin = pos;
}


ItemObject::~ItemObject(void)
{
}

bool ItemObject::Init()
{
	//!初期位置セット
	TEX_DRAW_INFO &drawInfo = m_drawTexture.m_pTex2D->UpdateDrawInfo();
	drawInfo.m_fileName = GetItemFilePath().c_str();
	if( drawInfo.m_posOrigin == DEFAULT_VECTOR2 ){
		drawInfo.m_posOrigin = Utility::GetMapRandamPos( /*allowInWindow=*/false );
	}
	drawInfo.m_prioity = Common::PRIORITY_BELOW_NORMAL;
	m_drawTexture.m_pTex2D->SetDrawInfo( drawInfo );

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
	case Common::ITEM_KIND_RAPID_BULLET:
		type = Common::TYPE_ITEM_BULLET;
		break;
	case Common::ITEM_KIND_LIFE_UP:
		type = Common::TYPE_ITEM_LIFE;
		break;
	case Common::ITEM_KIND_DAMAGE_UP:
		type = Common::TYPE_ITEM_DAMAGE;
		break;
	case Common::ITEM_KIND_ANTIDOTE:
		type = Common::TYPE_ITEM_ANTIDOTE;
		break;
	};
	return type;
}

/* ================================================ */
/**
 * @brief	イベント処理
 */
/* ================================================ */
void ItemObject::EventUpdate( Common::CMN_EVENT &eventId )
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
	case Common::ITEM_KIND_RAPID_BULLET:
		fileName = "ItemBullet.json";
		break;
	case Common::ITEM_KIND_LIFE_UP:
		fileName = "ItemLife.json";
		break;
	case Common::ITEM_KIND_DAMAGE_UP:
		fileName = "ItemDamage.json";
		break;
	case Common::ITEM_KIND_ANTIDOTE:
		fileName = "ItemAntidote.json";
		break;
	};

	return fileName;
}