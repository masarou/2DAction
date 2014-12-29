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
#include "Game/Player/GamePlayer.h"
#include "ItemObject.h"

class ItemManager : public TaskUnit
{

public:

	static ItemManager *CreateItemManager();
	~ItemManager(void);

	virtual void Update() override;
	virtual void CollisionUpdate() override;
	virtual void DrawUpdate() override;
	virtual bool DieMain() override;

	// アイテム生成
	void CreateItem( const ItemObject::ITEM_KIND &kind );
	void CreateItem( const ItemObject::ITEM_KIND &kind, math::Vector2 pos );
	// アイテムの削除
	void DeleteItem( uint32_t uniqueNumber );
	// 生成しているアイテムをカウント
	uint32_t CountItem();

private:

	ItemManager(void);
	bool CheckCollisionToPlayer( GamePlayer *player ) const;	// 当たり判定関数
	
	std::vector<ItemObject*>	m_itemArray;		// アイテムの管理ベクタ

};

#endif