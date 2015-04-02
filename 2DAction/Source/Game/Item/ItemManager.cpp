/* ====================================================================== */
/**
 * @brief  アイテム全般を管理するクラス
 *
 * @note
 *		
 */
/* ====================================================================== */

#include "ItemManager.h"
#include "Game/GameRegister.h"
#include "System/Sound/SystemSoundManager.h"
#include "Common/Utility/CommonGameUtility.h"


ItemManager *ItemManager::CreateItemManager()
{
	return NEW ItemManager();
}

ItemManager::ItemManager(void)
: TaskUnit("ItemManager")
{
}


ItemManager::~ItemManager(void)
{
}

bool ItemManager::DieMain()
{
	// 保持しているクラスをすべて除外
	m_itemArray.clear();
	return true;
}

/* ================================================ */
/**
 * @brief	アイテム生成
 */
/* ================================================ */
void ItemManager::CreateItem( const Common::ITEM_KIND &kind, math::Vector2 pos )
{
	ItemObject *item = ItemObject::Create( kind, pos );
	m_itemArray.push_back( item );
}

void ItemManager::RemoveItem( ItemObject *removeItem )
{
	for( auto it = m_itemArray.begin(); it != m_itemArray.end() ; ++it ){
		if( (*it) == removeItem ){
			m_itemArray.erase(it);
			break;
		}
	}
}
