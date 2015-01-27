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
 * @brief	�ʒu�A�`��X�V�֐�
 */
/* ================================================ */
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
		tmp.m_posOrigin = GetPlayerPos();
		if( IsInRangeTexture( tmp, m_itemArray.at(i)->GetDrawInfo() ) ){
			m_itemArray.at(i)->SetPlayerGetFlag();
			isHit = true;
		}
	}
	return true;
}

