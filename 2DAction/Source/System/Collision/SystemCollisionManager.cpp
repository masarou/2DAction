/* ====================================================================== */
/**
 * @brief  SystemCollisionManager.h
 *
 * @note	�����蔻����ꊇ�ōs���N���X
 *		
 */
/* ====================================================================== */

#include "SystemCollisionManager.h"
#include "System/Message/SystemMessageManager.h"
#include "Common/Utility/CommonGameUtility.h"

CollisionManager *CollisionManager::s_pInstance	= NULL;

CollisionManager *CollisionManager::Create()
{
	if( s_pInstance ){
		DEBUG_ASSERT( 0, "CollisionManager is already created");
	}
	else{
		s_pInstance = NEW CollisionManager();
	}
	return s_pInstance;
}

CollisionManager *CollisionManager::GetInstance()
{
	if( !s_pInstance ){
		DEBUG_ASSERT( 0, "CollisionManager is NULL");
	}
	return s_pInstance;
}

CollisionManager::CollisionManager(void)
{
}


CollisionManager::~CollisionManager(void)
{
}

/* ================================================ */
/**
 * @brief	�Ǘ����j�b�g�̓o�^�ƍ폜
 */
/* ================================================ */
void CollisionManager::AddUnit( Collision2DUnit *pUnit )
{
	for( uint32_t i = 0; i < m_vCollisionUnit.size() ; ++i ){
		if( m_vCollisionUnit.at(i) == pUnit ){
			DEBUG_ASSERT( 0, "���j�b�g�̏d���o�^!!!");
			return;
		}
	}
	m_vCollisionUnit.push_back( pUnit );
}
void CollisionManager::RemoveUnit( Collision2DUnit *pUnit )
{
	auto it = m_vCollisionUnit.begin();
	for(; it != m_vCollisionUnit.end() ; ++it){
		if( (*it) == pUnit ){
			// �Ǘ�������O��
			m_vCollisionUnit.erase(it);
			return;
		}
	}
}

/* ================================================ */
/**
 * @brief	�o�^���j�b�g�̓����蔻��J�n
 */
/* ================================================ */
void CollisionManager::CollisionUpdate()
{
	for( uint32_t i = 0; i < m_vCollisionUnit.size() ; ++i ){
		for( uint32_t j = 0; j < m_vCollisionUnit.size() ; ++j ){
			if( i == j ){
				// �����̓`�F�b�N����͂���
				continue;
			}

			Common::TYPE_OBJECT typeA = m_vCollisionUnit.at(i)->GetTypeObject();
			Common::TYPE_OBJECT typeB = m_vCollisionUnit.at(j)->GetTypeObject();
			if( typeA == Common::TYPE_PLAYER && typeB == Common::TYPE_BULLET_PLAYER ){
				continue;
			}
			if( typeA == Common::TYPE_BULLET_PLAYER && typeB == Common::TYPE_PLAYER ){
				continue;
			}

			const TEX_DRAW_INFO &texA = m_vCollisionUnit.at(i)->GetDrawInfo();
			const TEX_DRAW_INFO &texB = m_vCollisionUnit.at(j)->GetDrawInfo();
			if( IsInRangeTexture( texA, texB ) ){
				Common::CMN_EVENT eventInfo;
				eventInfo.Init();

				Common::EVENT_MESSAGE messageKind = Common::EVENT_MESSAGE_MAX;
				switch( typeA ){
				default:
					DEBUG_ASSERT( 0, "object�̎�ނ��z��O");
					/* fall-through */
				case Common::TYPE_PLAYER:
					messageKind = Common::EVENT_HIT_PLAYER;
					break;
				case Common::TYPE_EVENMY_AAA:
					messageKind = Common::EVENT_HIT_ENEMY_AAA;
					break;
				case Common::TYPE_EVENMY_BBB:
					messageKind = Common::EVENT_HIT_ENEMY_BBB;
					break;
				case Common::TYPE_EVENMY_CCC:
					messageKind = Common::EVENT_HIT_ENEMY_CCC;
					break;
				case Common::TYPE_ITEM:
					messageKind = Common::EVENT_GET_ITEM;
					break;
				case Common::TYPE_BULLET_PLAYER:
					messageKind = Common::EVENT_HIT_BULLET_PLAYER;
					break;
				case Common::TYPE_BULLET_ENEMY:
					messageKind = Common::EVENT_HIT_BULLET_ENEMY;
					break;
				}
				eventInfo.m_event = messageKind;

				// �ڐG�������Ƃ�`����
				SystemMessageManager::GetInstance()->PushMessage( m_vCollisionUnit.at(j)->GetUniqueId(), eventInfo );
			}
		}
	}
}

/* ================================================ */
/**
 * @brief	���̃N���X�̍폜
 */
/* ================================================ */
void CollisionManager::DeleteCollisionManager()
{
	DEBUG_PRINT("/_/_/SystemMessageManager �폜/_/_/\n");

	SAFE_DELETE( s_pInstance );
	
	DEBUG_PRINT("/_/_/SystemMessageManager ����/_/_/\n");
}