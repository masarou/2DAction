/* ====================================================================== */
/**
 * @brief  �A�C�e���S�ʂ��Ǘ�����N���X
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
	// �ێ����Ă���N���X�����ׂď��O
	m_itemArray.clear();
	return true;
}

/* ================================================ */
/**
 * @brief	�A�C�e������
 */
/* ================================================ */
void ItemManager::CreateItem( const ItemObject::ITEM_KIND &kind, math::Vector2 pos )
{
	static uint32_t uniqueNum = 0;
	ItemObject *item = ItemObject::Create( ItemObject::ITEM_KIND_RAPID_BULLET, uniqueNum, pos );
	m_itemArray.push_back( item );
	++uniqueNum;
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
