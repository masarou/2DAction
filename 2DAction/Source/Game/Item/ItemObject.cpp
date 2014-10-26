/* ====================================================================== */
/**
 * @brief  フィールドに無作為に出てくるアイテムクラス
 *
 * @note
 *		
 */
/* ====================================================================== */

#include "ItemObject.h"
#include "System/Sound/SystemSoundManager.h"

ItemObject::ItemObject( const ITEM_KIND &kind, const uint32_t &uniqueId, const math::Vector2 &pos )
: m_isPlayerGet(false)
, m_kindItem(kind)
, m_uniqueNumber(uniqueId)
, m_liveTime(0)
, m_drawItem(NULL)
{
	m_drawItem = NEW Game2DBase( GetItemFilePath().c_str() );

	//!初期位置セット
	m_itemInfo.Init();
	m_itemInfo.m_fileName = GetItemFilePath().c_str();
	m_itemInfo.m_pos = pos;
}


ItemObject::~ItemObject(void)
{
	SAFE_DELETE(m_drawItem);
}

void ItemObject::Update()
{
	// 生存時間更新
	++m_liveTime;
}

void ItemObject::Draw()
{
	// 消える三秒前ぐらいから点滅させる
	if( m_liveTime > ITEM_LIVE_TIME - 180 ){
		if( m_liveTime%3 != 1 ){
			m_drawItem->DrawUpdate2D();
		}
	}
	else{
		// アイテム描画
		m_drawItem->DrawUpdate2D();
	}
}

void ItemObject::SetPlayerGetFlag()
{
	m_liveTime = ITEM_LIVE_TIME;
	m_isPlayerGet = true;

	// 取得音を鳴らす
	SoundManager::GetInstance()->PlaySE("ShootBullet");
}

const TEX_DRAW_INFO &ItemObject::GetDrawInfo()
{
	return m_drawItem->GetDrawInfo();
}


std::string ItemObject::GetItemFilePath()
{
	std::string fileName = "";
	switch( m_kindItem ){
	default:
	case ITEM_RAPID_BULLET:
	case ITEM_LIFE_UP:
	case ITEM_DAMAGE_UP:
		fileName = "bullet.json";
		break;
	};

	return fileName;
}