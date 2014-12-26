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

void ItemManager::CollisionUpdate()
{
	GamePlayer *pPlayer = GameRegister::GetInstance()->UpdatePlayer();
	if( !pPlayer ){
		return ;
	}

	bool isHit = CheckCollisionToPlayer( pPlayer );
	if( isHit ){
		// �擾�����A�C�e���̐������C�x���gpush
		for( uint32_t i = 0; i < m_itemArray.size() ; ++i ){
			if( m_itemArray.at( i )->GetPlayerGetFlag() ){
				Common::CMN_EVENT hitEvent;
				hitEvent.Init();
				hitEvent.m_event		= Common::EVENT_GET_ITEM;
				hitEvent.m_eventValue	= static_cast<ItemObject::ITEM_KIND>( m_itemArray.at( i )->GetItemKind() );
				pPlayer->AddEvent( hitEvent );
			}
		}
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
 * @brief	�A�C�e������
 */
/* ================================================ */
void ItemManager::CreateItem( const ItemObject::ITEM_KIND &kind, math::Vector2 pos )
{
	static uint32_t uniqueNum = 0;
	ItemObject *item = NEW ItemObject( ItemObject::ITEM_RAPID_BULLET ,uniqueNum, pos );
	m_itemArray.push_back( item );
}

/* ================================================ */
/**
 * @brief	�A�C�e���̍폜(�v���C���[���Ƃ����A��莞�Ԃ��������X)
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

/* ================================================ */
/**
 * @brief	�w��v���C���[�Ƃ̓����蔻��`�F�b�N
 */
/* ================================================ */
bool ItemManager::CheckCollisionToPlayer( GamePlayer *player ) const
{
	bool isHit = false;
	for( uint32_t i = 0; i < m_itemArray.size() ; ++i){
		// �ʒu���ƃe�N�X�`���T�C�Y���܂߂ē������Ă��邩�ǂ���
		TEX_DRAW_INFO tmp = player->GetDrawInfo();
		tmp.m_posOrigin += GetPlayerOffsetPos();
		if( IsInRangeTexture( tmp, m_itemArray.at(i)->GetDrawInfo() ) ){
			m_itemArray.at(i)->SetPlayerGetFlag();
			isHit = true;
		}
	}
	return true;
}

