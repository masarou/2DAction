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

ItemObject *ItemObject::Create( const ITEM_KIND &kind, const uint32_t &uniqueID, math::Vector2 pos )
{
	return NEW ItemObject( kind, uniqueID, pos );
}

ItemObject::ItemObject( const ITEM_KIND &kind, const uint32_t &uniqueId, math::Vector2 pos )
: TaskUnit("ItemObject")
, m_isPlayerGet(false)
, m_kindItem(kind)
, m_uniqueNumber(uniqueId)
, m_liveTime(0)
{
	m_textureItem.Init();
	m_textureItem.m_texInfo.m_posOrigin = pos;
}


ItemObject::~ItemObject(void)
{
}

bool ItemObject::Init()
{
	m_textureItem.m_pTex2D = NEW Game2DBase( GetItemFilePath().c_str() );

	//!初期位置セット
	m_textureItem.m_texInfo.m_fileName = GetItemFilePath().c_str();
	if( m_textureItem.m_texInfo.m_posOrigin == DEFAULT_VECTOR2 ){
		m_textureItem.m_texInfo.m_posOrigin = GetMapRandamPos( /*allowInWindow=*/false );
	}
	m_textureItem.m_pTex2D->SetDrawInfo( m_textureItem.m_texInfo );

	return true;
}

bool ItemObject::DieMain()
{
	if( GameRegister::GetInstance()->GetManagerItem() ){
		GameRegister::GetInstance()->UpdateManagerItem()->RemoveItem( this );
	}

	m_textureItem.DeleteAndInit();
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
	if( m_liveTime > ITEM_LIVE_TIME - 180 ){
		if( m_liveTime%3 != 1 ){
			m_textureItem.m_pTex2D->DrawUpdate2D();
		}
	}
	else if( m_liveTime == ITEM_LIVE_TIME ){
		// 死亡
		TaskStartDie();
	}
	else{
		// アイテム描画
		m_textureItem.m_pTex2D->DrawUpdate2D();
	}
}

void ItemObject::SetPlayerGetFlag()
{
	m_liveTime = ITEM_LIVE_TIME;
	m_isPlayerGet = true;

	// 取得音を鳴らす
	SoundManager::GetInstance()->PlaySE("ShootBullet");
}

const TEX_DRAW_INFO &ItemObject::GetDrawInfo() const
{
	if( m_textureItem.m_pTex2D == NULL ){
		DEBUG_ASSERT( 0, "アイテムの描画クラスがNULL");
	}
	return m_textureItem.m_pTex2D->GetDrawInfo();
}


std::string ItemObject::GetItemFilePath()
{
	std::string fileName = "";
	switch( m_kindItem ){
	default:
	case ITEM_KIND_RAPID_BULLET:
	case ITEM_KIND_LIFE_UP:
	case ITEM_KIND_DAMAGE_UP:
		fileName = "bullet.json";
		break;
	};

	return fileName;
}