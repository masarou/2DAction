/* ====================================================================== */
/**
 * @brief  SystemCollisionManager.h
 *
 * @note	当たり判定を一括で行うクラス
 *		
 */
/* ====================================================================== */

#include "SystemCollisionManager.h"
#include "System/Message/SystemMessageManager.h"
#include "Game/Player/AttackGun/Bullet.h"
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
 * @brief	管理ユニットの登録と削除
 */
/* ================================================ */
void CollisionManager::AddUnit( Collision2DUnit *pUnit )
{
	for( uint32_t i = 0; i < m_vCollisionUnit.size() ; ++i ){
		if( m_vCollisionUnit.at(i) == pUnit ){
			DEBUG_ASSERT( 0, "ユニットの重複登録!!!");
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
			// 管理下から外す
			m_vCollisionUnit.erase(it);
			return;
		}
	}
}

/* ================================================ */
/**
 * @brief	登録ユニットの当たり判定開始
 */
/* ================================================ */
void CollisionManager::CollisionUpdate()
{
	for( uint32_t i = 0; i < m_vCollisionUnit.size() ; ++i ){
		for( uint32_t j = 0; j < m_vCollisionUnit.size() ; ++j ){
			if( i == j ){
				// 自分はチェックからはずす
				continue;
			}

			Common::TYPE_OBJECT typeA = m_vCollisionUnit.at(i)->GetTypeObject();
			Common::TYPE_OBJECT typeB = m_vCollisionUnit.at(j)->GetTypeObject();
			if( !NeedEvent( typeA, typeB ) ){
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
					DEBUG_ASSERT( 0, "objectの種類が想定外");
					/* fall-through */
				case Common::TYPE_PLAYER:
					messageKind = Common::EVENT_HIT_PLAYER;
					break;
				case Common::TYPE_EVENMY_AAA:
					messageKind = Common::EVENT_HIT_ENEMY_AAA;
					eventInfo.m_eventValue = 20;
					break;
				case Common::TYPE_EVENMY_BBB:
					messageKind = Common::EVENT_HIT_ENEMY_BBB;
					eventInfo.m_eventValue = 20;
					break;
				case Common::TYPE_EVENMY_CCC:
					messageKind = Common::EVENT_HIT_ENEMY_CCC;
					break;
				case Common::TYPE_ITEM_BULLET:
					messageKind = Common::EVENT_GET_ITEM_BULLET;
					break;
				case Common::TYPE_ITEM_LIFE:
					messageKind = Common::EVENT_GET_ITEM_LIFE;
					break;
				case Common::TYPE_ITEM_DAMAGE:
					messageKind = Common::EVENT_GET_ITEM_DAMAGE;
					break;
				case Common::TYPE_BULLET_PLAYER:
					{
						messageKind = Common::EVENT_HIT_BULLET_PLAYER;
						Bullet *pBullet = static_cast<Bullet*>( m_vCollisionUnit.at(i) );
						eventInfo.m_eventValue = pBullet->GetBulletDamage();
					}
					break;
				case Common::TYPE_BULLET_ENEMY:
					{
						messageKind = Common::EVENT_HIT_BULLET_ENEMY;
						Bullet *pBullet = static_cast<Bullet*>( m_vCollisionUnit.at(i) );
						eventInfo.m_eventValue = pBullet->GetBulletDamage();
					}
					break;
				}
				eventInfo.m_event = messageKind;

				// 接触したことを伝える
				SystemMessageManager::GetInstance()->PushMessage( m_vCollisionUnit.at(j)->GetUniqueId(), eventInfo );
			}
		}
	}
}

/* ================================================ */
/**
 * @brief	このクラスの削除
 */
/* ================================================ */
void CollisionManager::DeleteCollisionManager()
{
	DEBUG_PRINT("/_/_/SystemMessageManager 削除/_/_/\n");

	SAFE_DELETE( s_pInstance );
	
	DEBUG_PRINT("/_/_/SystemMessageManager 完了/_/_/\n");
}

/* ================================================ */
/**
 * @brief	そもそも衝突通知をする必要があるかどうかチェック
 */
/* ================================================ */
bool CollisionManager::NeedEvent( const Common::TYPE_OBJECT typeA, const Common::TYPE_OBJECT typeB ) const
{
	// 同じ種類なら通知不要
	if( typeA == typeB ){
		return false;
	}

	bool retVal = true;

	switch( typeA ){
	default:
		DEBUG_ASSERT( 0, "オブジェクトのタイプが想定外" );
		break;
	case Common::TYPE_PLAYER:
		if( typeB == Common::TYPE_BULLET_PLAYER ){
			retVal = false;
		}
		break;
	case Common::TYPE_EVENMY_AAA:
	case Common::TYPE_EVENMY_BBB:
	case Common::TYPE_EVENMY_CCC:
		if( typeB != Common::TYPE_PLAYER
			&& typeB != Common::TYPE_BULLET_PLAYER ){
			retVal = false;
		}
		break;
	case Common::EVENT_GET_ITEM_BULLET:
	case Common::EVENT_GET_ITEM_LIFE:
	case Common::EVENT_GET_ITEM_DAMAGE:
	case Common::TYPE_BULLET_ENEMY:
		if( typeB != Common::TYPE_PLAYER ){
			retVal = false;
		}
		break;
	case Common::TYPE_BULLET_PLAYER:
		if( typeB == Common::TYPE_PLAYER
			|| typeB == Common::EVENT_GET_ITEM_BULLET
			|| typeB == Common::EVENT_GET_ITEM_LIFE
			|| typeB == Common::EVENT_GET_ITEM_DAMAGE
			|| typeB == Common::TYPE_BULLET_PLAYER){
			retVal = false;
		}
		break;
	}

	return retVal;
}