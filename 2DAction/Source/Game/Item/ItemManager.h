/* ====================================================================== */
/**
 * @brief  �A�C�e���S�ʂ��Ǘ�����N���X
 *
 * @note
 *		
 */
/* ====================================================================== */

#ifndef __SYSTEM_ITEM_MANAGER__
#define __SYSTEM_ITEM_MANAGER__

#include "System/Task/SystemTaskUnit.h"
#include "Game/Player/GamePlayer.h"
#include "ItemObject.h"

class ItemManager : public TaskUnit
{

public:

	static ItemManager *CreateItemManager();
	
	void CreateItem( const ItemObject::ITEM_KIND &kind, math::Vector2 pos = DEFAULT_VECTOR2 );	// �A�C�e������
	void RemoveItem( ItemObject *removeItem );	// �A�C�e���̍폜
		
	uint32_t CountItem() const{ return m_itemArray.size(); }

protected:

	virtual bool DieMain() override;

private:

	ItemManager(void);
	~ItemManager(void);
	
	std::vector<ItemObject*>	m_itemArray;		// �A�C�e���̊Ǘ��x�N�^
};

#endif