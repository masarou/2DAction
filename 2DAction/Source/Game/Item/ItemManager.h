/* ====================================================================== */
/**
 * @brief  アイテム全般を管理するクラス
 *
 * @note
 *		
 */
/* ====================================================================== */

#ifndef SYSTEM_ITEM_MANAGER
#define SYSTEM_ITEM_MANAGER

#include "System/Task/SystemTaskUnit.h"
#include "ItemObject.h"

class ItemManager : public TaskUnit
{

public:

	ItemManager(void);
	~ItemManager(void);

	virtual void Update() override;
	virtual void DrawUpdate() override;
	virtual bool DieMain() override;

	// 弾の発射
	void CreateItem( const ItemObject::ITEM_KIND &kind, math::Vector2 pos = math::Vector2() );
	// 弾の削除(画面外に出た、敵に当たった等々)
	void DeleteItem( uint32_t uniqueNumber );

private:

	std::vector<ItemObject*>	m_itemArray;		// アイテムの管理ベクタ

};

#endif