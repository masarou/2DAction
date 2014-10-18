/* ====================================================================== */
/**
 * @brief  �A�C�e���S�ʂ��Ǘ�����N���X
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
	// �ێ����Ă���N���X�����ׂĉ��
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
 * @brief	�ʒu�A�`��X�V�֐�
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

	// ��������Ĉ�莞�Ԃ𒴂������͍̂폜
	for( uint32_t i = 0; i < m_itemArray.size() ; ++i ){
		if( m_itemArray.at( i )->GetLiveTime() >= ItemObject::ITEM_LIVE_TIME ){
			DeleteItem( m_itemArray.at( i )->GetUniqueNumber() );
		}
	}
}


/* ================================================ */
/**
 * @brief	�e�̔���
 */
/* ================================================ */
void ItemManager::CreateItem( const ItemObject::ITEM_KIND &kind, math::Vector2 pos )
{
	static uint32_t uniqueNum = 0;
	ItemObject *item = NEW ItemObject( ItemObject::ITEM_RAPID_BULLET ,uniqueNum, pos );
	m_itemArray.push_back( item );
		
	// ���ˉ���炷
	SoundManager::GetInstance()->PlaySE("ShootBullet");
}

/* ================================================ */
/**
 * @brief	�e�̍폜(��ʊO�ɏo���A�G�ɓ����������X)
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

