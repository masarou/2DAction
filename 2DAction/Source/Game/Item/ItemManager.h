/* ====================================================================== */
/**
 * @brief  アイテム全般を管理するクラス
 *
 * @note
 *		アイテムの生成は必ずGameManager(friendクラス)を通して行われます
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
	
	void RemoveItem( ItemObject *removeItem );	// アイテムの削除

	uint32_t CountItem() const{ return m_itemArray.size(); }

protected:

	virtual bool DieMain() override;

private:

	void CreateItem( const Common::ITEM_KIND &kind, math::Vector2 pos = DEFAULT_VECTOR2 );	// アイテム生成

	ItemManager(void);
	~ItemManager(void);
	
	std::vector<ItemObject*>	m_itemArray;		// アイテムの管理ベクタ
};

#endif