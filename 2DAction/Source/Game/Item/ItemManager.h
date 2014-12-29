/* ====================================================================== */
/**
 * @brief  �A�C�e���S�ʂ��Ǘ�����N���X
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

	// �A�C�e������
	void CreateItem( const ItemObject::ITEM_KIND &kind );
	void CreateItem( const ItemObject::ITEM_KIND &kind, math::Vector2 pos );
	// �A�C�e���̍폜
	void DeleteItem( uint32_t uniqueNumber );
	// �������Ă���A�C�e�����J�E���g
	uint32_t CountItem();

private:

	ItemManager(void);
	bool CheckCollisionToPlayer( GamePlayer *player ) const;	// �����蔻��֐�
	
	std::vector<ItemObject*>	m_itemArray;		// �A�C�e���̊Ǘ��x�N�^

};

#endif