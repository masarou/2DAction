/* ====================================================================== */
/**
 * @brief  敵管理クラス
 *
 * @note
 *		
 */
/* ====================================================================== */

#include "EnemyManager.h"
#include "EnemyAAA.h"
#include "EnemyBBB.h"
#include "Game/Player/AttackGun/Bullet.h"
#include "Game/Player/GamePlayer.h"
#include "Game/GameRegister.h"
#include "Common/Utility/CommonGameUtility.h"

EnemyManager *EnemyManager::CreateEnemyManager()
{
	return NEW EnemyManager();
}

EnemyManager::EnemyManager(void)
: TaskUnit("EnemyManager")
{
}

EnemyManager::~EnemyManager(void)
{
}

bool EnemyManager::DieMain()
{
	// 保持しているクラスをすべて除外
	m_enemyArray.clear();

	return true;
}

/* ================================================ */
/**
 * @brief	敵クラスの追加と除外
 */
/* ================================================ */
void EnemyManager::AddEnemy( const Common::ENEMY_KIND &kind )
{
	static uint32_t currUniqueNo = 0;
	EnemyBase *pEnemy = NULL;
	switch( kind ){
	case Common::ENEMY_KIND_AAA:
		pEnemy = EnemyAAA::Create( currUniqueNo );
		break;
	case Common::ENEMY_KIND_BBB:
		pEnemy = EnemyBBB::Create( currUniqueNo );
		break;
	case Common::ENEMY_KIND_CCC:
		pEnemy = EnemyAAA::Create( currUniqueNo );
		break;
	case Common::ENEMY_KIND_DDD:
		pEnemy = EnemyAAA::Create( currUniqueNo );
		break;

	default:
		DEBUG_ASSERT( 0,  "敵の種類が想定外" );
		break;
	}
	if( pEnemy ){
		m_enemyArray.push_back( pEnemy );
		++currUniqueNo;
	}
}

void EnemyManager::RemoveEnemy( EnemyBase *removeEnemy )
{
	for( auto it = m_enemyArray.begin(); it != m_enemyArray.end() ; ++it ){
		if( (*it) == removeEnemy ){
			m_enemyArray.erase(it);
			break;
		}
	}
}

/* ================================================ */
/**
 * @brief	位置、描画更新関数
 */
/* ================================================ */
void EnemyManager::CollisionUpdate()
{
	//GamePlayer *pPlayer = GameRegister::GetInstance()->UpdatePlayer();
	//if( !pPlayer ){
	//	return ;
	//}

	//bool isHit = CheckCollisionToPlayer( pPlayer );
	//if( isHit ){
	//	// 取得したアイテムの数だけイベントpush
	//	for( uint32_t i = 0; i < m_itemArray.size() ; ++i ){
	//		if( m_itemArray.at( i )->GetPlayerGetFlag() ){
	//			Common::CMN_EVENT hitEvent;
	//			hitEvent.Init();
	//			hitEvent.m_event		= Common::EVENT_GET_ITEM;
	//			hitEvent.m_eventValue	= static_cast<ItemObject::ITEM_KIND>( m_itemArray.at( i )->GetItemKind() );
	//			pPlayer->AddEvent( hitEvent );
	//		}
	//	}
	//}
}

/* ================================================ */
/**
 * @brief	敵クラスとの当たり判定関数
 */
/* ================================================ */
bool EnemyManager::CheckCollision( const TEX_DRAW_INFO &texInfo ) const
{
	bool isHit = false;
	for( uint32_t i = 0; i < m_enemyArray.size() ; ++i){
		
		// 初期化中だったら無視
		if( m_enemyArray.at(i)->GetStatus() == TaskUnit::TASK_INIT ){
			continue;
		}

		// 位置情報とテクスチャサイズを含めて当たっているかどうか
		if( IsInRangeTexture( texInfo, m_enemyArray.at(i)->GetDrawInfo() ) ){
			isHit = true;
			break;
		}
	}
	return isHit;
}

//bool EnemyManager::CheckCollisionToBullet( const Bullet *bullet )
//{
//	bool isHit = false;
//	for( uint32_t i = 0; i < m_enemyArray.size() ; ++i){
//
//		// 初期化中だったら無視
//		if( m_enemyArray.at(i)->GetStatus() == TaskUnit::TASK_INIT ){
//			continue;
//		}
//
//		// 位置情報とテクスチャサイズを含めて当たっているかどうか
//		if( IsInRangeTexture( bullet->GetDrawInfo(), m_enemyArray.at(i)->GetDrawInfo() ) ){
//			Common::CMN_EVENT eventInfo;
//			eventInfo.Init();
//			eventInfo.m_event = Common::EVENT_HIT_BULLET;
//			eventInfo.m_eventValue = m_enemyArray.at(i)->GetUniqueNumber();
//			SystemMessageManager::GetInstance()->PushMessage( m_enemyArray.at(i)->GetUniqueId(), eventInfo );
//			isHit = true;
//			break;
//		}
//	}
//	return isHit;
//}
//
//bool EnemyManager::CheckCollisionToPlayer( const GamePlayer *player ) const
//{
//	bool isHit = false;
//	for( uint32_t i = 0; i < m_enemyArray.size() ; ++i){
//
//		// 初期化中だったら無視
//		if( m_enemyArray.at(i)->GetStatus() == TaskUnit::TASK_INIT ){
//			continue;
//		}
//
//		// 位置情報とテクスチャサイズを含めて当たっているかどうか
//		TEX_DRAW_INFO tmp = player->GetDrawInfo();
//		tmp.m_posOrigin = GetPlayerPos();
//		if( IsInRangeTexture( tmp, m_enemyArray.at(i)->GetDrawInfo() ) ){
//			isHit = true;
//		}
//	}
//	return isHit;
//}
