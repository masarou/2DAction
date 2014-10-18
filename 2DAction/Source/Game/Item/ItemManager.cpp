/* ====================================================================== */
/**
 * @brief  アイテム全般を管理するクラス
 *
 * @note
 *		
 */
/* ====================================================================== */

#include "ItemManager.h"
#include "System/Sound/SystemSoundManager.h"

ItemManager::ItemManager(void)
{
}


ItemManager::~ItemManager(void)
{
}

bool ItemManager::DieMain()
{
	// 保持しているクラスをすべて解放
	std::vector<ItemObject*>::iterator it = m_itemArray.begin();
	while(m_itemArray.empty() == 0){
		ItemObject *Item = (*it);
		it = m_itemArray.erase(it);
		SAFE_DELETE(Item);
	}
	m_itemArray.clear();
	return true;
}

/* ================================================ */
/**
 * @brief	位置、描画更新関数
 */
/* ================================================ */
void ItemManager::Update()
{
	for( uint32_t i = 0; i < m_itemArray.size() ; ++i ){
		m_itemArray.at( i )->Update();
	}
}

void ItemManager::DrawUpdate()
{
	for( uint32_t i = 0; i < m_itemArray.size() ; ++i ){
		m_itemArray.at( i )->Draw();
	}

	// 生成されて一定時間を超えたものは削除
	for( uint32_t i = 0; i < m_itemArray.size() ; ++i ){
		if( m_itemArray.at( i )->GetLiveTime() >= ItemObject::ITEM_LIVE_TIME ){
			DeleteItem( m_itemArray.at( i )->GetUniqueNumber() );
		}
	}
}


/* ================================================ */
/**
 * @brief	弾の発射
 */
/* ================================================ */
void ItemManager::CreateItem( const ItemObject::ITEM_KIND &kind, math::Vector2 pos )
{
	static uint32_t uniqueNum = 0;
	ItemObject *item = NEW ItemObject( ItemObject::ITEM_RAPID_BULLET ,uniqueNum, pos );
	m_itemArray.push_back( item );
		
	// 発射音を鳴らす
	SoundManager::GetInstance()->PlaySE("ShootBullet");
}

/* ================================================ */
/**
 * @brief	弾の削除(画面外に出た、敵に当たった等々)
 */
/* ================================================ */
void ItemManager::DeleteItem( uint32_t uniqueNumber )
{
	std::vector<ItemObject*>::iterator it = m_itemArray.begin();
	for( uint32_t i = 0; i < m_itemArray.size() ; ++i ){
		if( (*it)->GetUniqueNumber() == uniqueNumber ){
			ItemObject *pTmp = ( *it );
			m_itemArray.erase( it );
			SAFE_DELETE( pTmp );
			break;
		}
		++it;
	}
}

