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
#include "Game/Attack/Bullet.h"
#include "Game/Attack/Slashing.h"
#include "Common/Utility/CommonGameUtility.h"
#include "Game/GameMap.h"

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
	for( uint32_t i = 0; i < NUMBEROF( m_objectTree ) ; ++i ){
		m_objectTree[i].Init();
	}
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
			// 双方向リストからも外す
			//uint32_t index = GetRegisterTreeIndex( pUnit );
			//RemoveUnitFromTree( index, pUnit );
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
			if( m_vCollisionUnit.at(i)->GetInvalidCollisionFlag() || m_vCollisionUnit.at(j)->GetInvalidCollisionFlag() )
			{
				// 当たり判定無効フラグが立っている
				continue;
			}
			Common::TYPE_OBJECT typeA = m_vCollisionUnit.at(i)->GetTypeObject();
			Common::TYPE_OBJECT typeB = m_vCollisionUnit.at(j)->GetTypeObject();
			if( !NeedEvent( typeA, typeB ) ){
				// 当たり判定が不要なオブジェクトどうし
				continue;
			}
			const TEX_DRAW_INFO &texA = m_vCollisionUnit.at(i)->GetDrawInfo();
			const TEX_DRAW_INFO &texB = m_vCollisionUnit.at(j)->GetDrawInfo();
			if( Utility::IsInRangeTexture( texA, texB ) ){
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
					eventInfo.m_eventValue = 10;
					break;
				case Common::TYPE_EVENMY_BBB:
					messageKind = Common::EVENT_HIT_ENEMY_BBB;
					eventInfo.m_eventValue = 10;
					break;
				case Common::TYPE_EVENMY_CCC:
					messageKind = Common::EVENT_HIT_ENEMY_CCC;
					eventInfo.m_eventValue = 10;
					break;
				case Common::TYPE_EVENMY_BOSS:
					messageKind = Common::EVENT_HIT_ENEMY_BOSS;
					eventInfo.m_eventValue = 10;
					break;
				case Common::TYPE_EVENMY_SLIME_KING:
					messageKind = Common::EVENT_HIT_ENEMY_SLIME_KING;
					eventInfo.m_eventValue = 20;
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
				case Common::TYPE_ITEM_BATTLE_POINT:
					messageKind = Common::EVENT_GET_ITEM_BATTLE_POINT;
					break;
				case Common::TYPE_BLADE_PLAYER:
					{
						messageKind = Common::EVENT_HIT_BLADE_PLAYER;
						Slashing *pSlashing = static_cast<Slashing*>( m_vCollisionUnit.at(i) );
						eventInfo.m_eventValue = pSlashing->GetBladeDamage();
					}
					break;
				case Common::TYPE_BLADE_ENEMY:
					{
						messageKind = Common::EVENT_HIT_BLADE_ENEMY;
						Slashing *pSlashing = static_cast<Slashing*>( m_vCollisionUnit.at(i) );
						eventInfo.m_eventValue = pSlashing->GetBladeDamage();
					}
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
				case Common::TYPE_EXPLOSION_PLAYER:
					{
						messageKind = Common::EVENT_HIT_EXPLOSION_PLAYER;
						eventInfo.m_eventValue = 40;
					}
					break;
				case Common::TYPE_EXPLOSION_ENEMY:
					{
						messageKind = Common::EVENT_HIT_EXPLOSION_ENEMY;
						eventInfo.m_eventValue = 40;
					}
					break;
				}
				eventInfo.m_event = messageKind;

				// 接触したことを伝える
				SystemMessageManager::GetInstance()->PushMessage( m_vCollisionUnit.at(j)->GetUniqueId(), eventInfo );
			}
		}
	}

	//// とりあえず毎回初期化
	//for( uint32_t i = 0; i < m_vCollisionUnit.size() ; ++i ){
	//	m_vCollisionUnit.at( i )->ClearChainList();
	//}
	//for( uint32_t i = 0; i < NUMBEROF(m_objectTree) ; ++i ){
	//	m_objectTree[i].Init();
	//}

	//// 各Unitの双方向リストを成型
	//for( uint32_t i = 0; i < m_vCollisionUnit.size() ; ++i ){
	//	Collision2DUnit *pTmp = m_vCollisionUnit.at( i );
	//	std::vector<uint32_t> indexArray;
	//	uint32_t accessIndex = GetRegisterTreeIndex( pTmp );
	//	if( accessIndex != INVALID_VALUE ){
	//		RegisterUnitFromTree( accessIndex, pTmp );
	//	}
	//}

	//// 当たり判定全チェック
	//for( uint32_t i = 0; i < NUMBEROF(m_objectTree) ; ++i ){
	//	if( m_objectTree[i].pUnit ){

	//		// 再帰
	//		RecursiveSameAreaCheck( m_objectTree[i].pUnit );

	//		// チェック終了次へ
	//		m_vCheckCollision.clear();

	//	}
	//}
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
		if( typeB == Common::TYPE_BULLET_PLAYER
			|| typeB == Common::TYPE_BLADE_PLAYER ){
			retVal = false;
		}
		break;
	case Common::TYPE_EVENMY_AAA:
	case Common::TYPE_EVENMY_BBB:
	case Common::TYPE_EVENMY_CCC:
	case Common::TYPE_EVENMY_BOSS:
	case Common::TYPE_EVENMY_SLIME_KING:
		if( typeB != Common::TYPE_PLAYER
			&& typeB != Common::TYPE_BULLET_PLAYER
			&& typeB != Common::TYPE_BLADE_PLAYER ){
			retVal = false;
		}
		break;
	case Common::TYPE_ITEM_BULLET:
	case Common::TYPE_ITEM_LIFE:
	case Common::TYPE_ITEM_DAMAGE:
	case Common::TYPE_ITEM_BATTLE_POINT:
	case Common::TYPE_BULLET_ENEMY:
	case Common::TYPE_BLADE_ENEMY:
	//case Common::TYPE_EXPLOSION_ENEMY:
		if( typeB != Common::TYPE_PLAYER ){
			retVal = false;
		}
		break;
	case Common::TYPE_BLADE_PLAYER:
	case Common::TYPE_BULLET_PLAYER:
	case Common::TYPE_EXPLOSION_PLAYER:
		if( typeB == Common::TYPE_PLAYER
			|| typeB == Common::TYPE_ITEM_BULLET
			|| typeB == Common::TYPE_ITEM_LIFE
			|| typeB == Common::TYPE_ITEM_DAMAGE
			|| typeB == Common::TYPE_BLADE_PLAYER
			|| typeB == Common::TYPE_BULLET_PLAYER){
			retVal = false;
		}
		break;
	case Common::TYPE_EXPLOSION_ENEMY:
		{uint32_t aaa = 0;}
		break;
	}

	return retVal;
}

// 所属Lvとその空間のIndex番号から「m_objectTree」の配列番号を求める
uint32_t CollisionManager::GetRegisterTreeIndex( const Collision2DUnit *pUnit ) const
{
	if( !pUnit ){
		DEBUG_ASSERT( 0, "与えられたpUnitがNULL" );
		return INVALID_VALUE;
	}

	const uint32_t &belongLv = pUnit->GetBelongLv();
	const uint32_t &belongIndex = pUnit->GetBelongIndex();
	return GetRegisterTreeIndex( belongLv, belongIndex );
}
uint32_t CollisionManager::GetRegisterTreeIndex( const uint32_t &belongLv, const uint32_t &belongIndex ) const
{
	uint32_t minimumLvIndex = static_cast<uint32_t>( ( pow( static_cast<double>(4), static_cast<int>(belongLv) ) - 1 ) / 3 );

	return minimumLvIndex+belongIndex;
}

// 指定クラスを「m_objectTree」に登録
void CollisionManager::RegisterUnitFromTree( const uint32_t &treeIndex, Collision2DUnit *pUnit )
{
	if( treeIndex >= NUMBEROF(m_objectTree) ){
		DEBUG_ASSERT( 0, "指定されたIndex番号が想定外");
		return;
	}
	m_objectTree[treeIndex].AddUnit( pUnit );
	++m_objectTree[treeIndex].registNum;
}

// 指定クラスを「m_objectTree」から削除
void CollisionManager::RemoveUnitFromTree( const uint32_t &treeIndex, Collision2DUnit *pUnit )
{
	if( treeIndex >= NUMBEROF(m_objectTree) ){
		DEBUG_ASSERT( 0, "指定されたIndex番号が想定外");
		return;
	}
	if( !pUnit ){
		DEBUG_ASSERT( 0, "pUnitがNULL");
		return;
	}
	if( !m_objectTree[treeIndex].pUnit ){
		// 特に何もする必要ないのでスルー
		return;
	}
	
	Collision2DUnit *pNextUnit	= m_objectTree[treeIndex].pUnit->GetNextUnit();
	Collision2DUnit *pCheckUnit	= m_objectTree[treeIndex].pUnit;
	Collision2DUnit *pPrevUnit	= m_objectTree[treeIndex].pUnit->GetPrevUnit();

	if( pNextUnit ){
		pNextUnit->SetPrevUnit( pPrevUnit );
	}
	if( pPrevUnit ){
		pPrevUnit->SetNextUnit( pNextUnit );
	}

	pCheckUnit->SetNextUnit( NULL );
	pCheckUnit->SetPrevUnit( NULL );

	--m_objectTree[treeIndex].registNum;
}

// 同じエリアのオブジェクトについて再帰的に当たり判定を行う
//void CollisionManager::RecursiveSameAreaCheck( Collision2DUnit *pUnit )
//{
//	uint32_t popCounter = 0;
//
//	// 連結しているUnitをまとめる
//	Collision2DUnit *pAddUnit = pUnit;
//	for(;;){
//		if( !pAddUnit ){
//			break;
//		}
//		m_vCheckCollision.push_back( pAddUnit );
//		pAddUnit = pAddUnit->GetNextUnit();
//		++popCounter;
//	}
//
//	// 連結していたUnitどうしの当たり判定チェック
//	Collision2DUnit *pCheckUnit = pUnit;
//	for(;;){
//		for( uint32_t j = 0; j < m_vCheckCollision.size() ; ++j ){
//			if( pCheckUnit != m_vCheckCollision.at(j) ){
//				CheckUnitCollision( pCheckUnit, m_vCheckCollision.at(j) );
//			}
//		}
//		pCheckUnit = pCheckUnit->GetNextUnit();
//		if( !pCheckUnit ){
//			break;
//		}
//	}
//
//	// 現在調べているエリアの子、孫とチェックしていく
//	if( pUnit->GetBelongLv() < HIT_AREA_SPLIT_MAX ){
//		// 子となる空間があるので再帰して調べていく		
//		uint32_t childLvFirstIndex = GetRegisterTreeIndex( pUnit->GetBelongLv() + 1, 0 );
//		childLvFirstIndex += 4 * pUnit->GetBelongIndex();
//		for( uint32_t i = childLvFirstIndex ; i < childLvFirstIndex+4 ; ++i ){
//			if( m_objectTree[i].pUnit ){
//				RecursiveSameAreaCheck( m_objectTree[i].pUnit );
//			}
//		}
//	}
//
//	for( uint32_t i = 0; i < popCounter; ++i ){
//		m_vCheckCollision.pop_back();
//	}
//}

// 当たり判定を実際に行う
//void CollisionManager::CheckUnitCollision( Collision2DUnit *unitA, Collision2DUnit *unitB )
//{
//	Common::TYPE_OBJECT typeA = unitA->GetTypeObject();
//	Common::TYPE_OBJECT typeB = unitB->GetTypeObject();
//	if( !NeedEvent( typeA, typeB ) ){
//		return;
//	}
//	const TEX_DRAW_INFO &texA = unitA->GetDrawInfo();
//	const TEX_DRAW_INFO &texB = unitB->GetDrawInfo();
//	if( Utility::IsInRangeTexture( texA, texB ) ){
//		Common::CMN_EVENT eventInfo;
//		eventInfo.Init();
//
//		Common::EVENT_MESSAGE messageKind = Common::EVENT_MESSAGE_MAX;
//		switch( typeA ){
//		default:
//			DEBUG_ASSERT( 0, "objectの種類が想定外");
//			/* fall-through */
//		case Common::TYPE_PLAYER:
//			messageKind = Common::EVENT_HIT_PLAYER;
//			break;
//		case Common::TYPE_EVENMY_AAA:
//			messageKind = Common::EVENT_HIT_ENEMY_AAA;
//			eventInfo.m_eventValue = 20;
//			break;
//		case Common::TYPE_EVENMY_BBB:
//			messageKind = Common::EVENT_HIT_ENEMY_BBB;
//			eventInfo.m_eventValue = 20;
//			break;
//		case Common::TYPE_EVENMY_CCC:
//			messageKind = Common::EVENT_HIT_ENEMY_CCC;
//			break;
//		case Common::TYPE_ITEM_BULLET:
//			messageKind = Common::EVENT_GET_ITEM_BULLET;
//			break;
//		case Common::TYPE_ITEM_LIFE:
//			messageKind = Common::EVENT_GET_ITEM_LIFE;
//			break;
//		case Common::TYPE_ITEM_DAMAGE:
//			messageKind = Common::EVENT_GET_ITEM_DAMAGE;
//			break;
//		case Common::TYPE_BLADE_PLAYER:
//			{
//				messageKind = Common::EVENT_HIT_BLADE_PLAYER;
//				eventInfo.m_eventValue = 20;
//			}
//			break;
//		case Common::TYPE_BLADE_ENEMY:
//			{
//				messageKind = Common::EVENT_HIT_BLADE_ENEMY;
//				eventInfo.m_eventValue = 20;
//			}
//		case Common::TYPE_BULLET_PLAYER:
//			{
//				messageKind = Common::EVENT_HIT_BULLET_PLAYER;
//				if( Bullet *pBullet = dynamic_cast<Bullet*>( unitA ) ){
//					eventInfo.m_eventValue = pBullet->GetBulletDamage();
//				}
//			}
//			break;
//		case Common::TYPE_BULLET_ENEMY:
//			{
//				messageKind = Common::EVENT_HIT_BULLET_ENEMY;
//				if( Bullet *pBullet = dynamic_cast<Bullet*>( unitA ) ){
//					eventInfo.m_eventValue = pBullet->GetBulletDamage();
//				}
//			}
//			break;
//		}
//		eventInfo.m_event = messageKind;
//
//		// 接触したことを伝える
//		SystemMessageManager::GetInstance()->PushMessage( unitB->GetUniqueId(), eventInfo );
//	}
//}
