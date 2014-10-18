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
#include "ItemObject.h"

class ItemManager : public TaskUnit
{

public:

	ItemManager(void);
	~ItemManager(void);

	virtual void Update() override;
	virtual void DrawUpdate() override;
	virtual bool DieMain() override;

	// �e�̔���
	void CreateItem( const ItemObject::ITEM_KIND &kind, math::Vector2 pos = math::Vector2() );
	// �e�̍폜(��ʊO�ɏo���A�G�ɓ����������X)
	void DeleteItem( uint32_t uniqueNumber );

private:

	std::vector<ItemObject*>	m_itemArray;		// �A�C�e���̊Ǘ��x�N�^

};

#endif