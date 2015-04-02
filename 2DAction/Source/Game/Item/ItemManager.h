/* ====================================================================== */
/**
 * @brief  �A�C�e���S�ʂ��Ǘ�����N���X
 *
 * @note
 *		�A�C�e���̐����͕K��GameManager(friend�N���X)��ʂ��čs���܂�
 */
/* ====================================================================== */

#ifndef __SYSTEM_ITEM_MANAGER__
#define __SYSTEM_ITEM_MANAGER__

#include "System/Task/SystemTaskUnit.h"
#include "Game/GameManager.h"
#include "Game/Player/GamePlayer.h"
#include "ItemObject.h"

class ItemManager : public TaskUnit
{

	friend class GameManager;

public:

	static ItemManager *CreateItemManager();
	
	void RemoveItem( ItemObject *removeItem );	// �A�C�e���̍폜

	uint32_t CountItem() const{ return m_itemArray.size(); }

protected:

	virtual bool DieMain() override;

private:

	void CreateItem( const Common::ITEM_KIND &kind, math::Vector2 pos = DEFAULT_VECTOR2 );	// �A�C�e������

	ItemManager(void);
	~ItemManager(void);
	
	std::vector<ItemObject*>	m_itemArray;		// �A�C�e���̊Ǘ��x�N�^
};

#endif