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
#include "Game/Player/AttackGun/Bullet.h"
#include "Game/Player/GamePlayer.h"
#include "Game/GameRegister.h"
#include "Common/Utility/CommonGameUtility.h"

EnemyManager *EnemyManager::CreateEnemyManager()
{
	return NEW EnemyManager();
}

EnemyManager::EnemyManager(void)
{
}

EnemyManager::~EnemyManager(void)
{
}

bool EnemyManager::DieMain()
{
	// 保持しているクラスをすべて解放
	std::vector<EnemyBase*>::iterator it = m_enemyArray.begin();
	while(m_enemyArray.empty() == 0){
		EnemyBase *enemy = (*it);
		it = m_enemyArray.erase(it);
		SAFE_DELETE(enemy);
	}
	m_enemyArray.clear();
	return true;
}

void EnemyManager::CreateEnemy( const Common::ENEMY_KIND &kind )
{
	static uint32_t currUniqueNo = 0;
	EnemyBase *pEnemy = NULL;
	switch( kind ){
	case Common::ENEMY_KIND_AAA:
		pEnemy = EnemyAAA::Create( currUniqueNo );
		break;
	case Common::ENEMY_KIND_BBB:
		pEnemy = EnemyAAA::Create( currUniqueNo );
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

void EnemyManager::DeleteEnemy( const uint32_t &uniqueNumber )
{
	std::vector<EnemyBase*>::iterator it = m_enemyArray.begin();
	for( uint32_t i = 0; i < m_enemyArray.size() ; ++i ){
		if( (*it)->GetUniqueNumber() == uniqueNumber ){
			// 削除リストにpushしておく後ほど削除
			m_delEnemyArray.push_back( ( *it ) );
			m_enemyArray.erase( it );
			break;
		}
		++it;
	}

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
		if( m_enemyArray.at(i)->GetState() == EnemyBase::ENEMY_INIT ){
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

bool EnemyManager::CheckCollisionToBullet( const Bullet *bullet )
{
	bool isHit = false;
	for( uint32_t i = 0; i < m_enemyArray.size() ; ++i){

		// 初期化中だったら無視
		if( m_enemyArray.at(i)->GetState() == EnemyBase::ENEMY_INIT ){
			continue;
		}

		// 位置情報とテクスチャサイズを含めて当たっているかどうか
		if( IsInRangeTexture( bullet->GetDrawInfo(), m_enemyArray.at(i)->GetDrawInfo() ) ){
			Common::CMN_EVENT eventInfo;
			eventInfo.Init();
			eventInfo.m_event = Common::EVENT_HIT_BULLET;
			eventInfo.m_eventValue = m_enemyArray.at(i)->GetUniqueNumber();
			SystemMessageManager::GetInstance()->PushMessage( m_enemyArray.at(i)->GetUniqueId(), eventInfo );
			isHit = true;
			break;
		}
	}
	return isHit;
}

bool EnemyManager::CheckCollisionToPlayer( const GamePlayer *player ) const
{
	bool isHit = false;
	for( uint32_t i = 0; i < m_enemyArray.size() ; ++i){

		// 初期化中だったら無視
		if( m_enemyArray.at(i)->GetState() == EnemyBase::ENEMY_INIT ){
			continue;
		}

		// 位置情報とテクスチャサイズを含めて当たっているかどうか
		TEX_DRAW_INFO tmp = player->GetDrawInfo();
		tmp.m_posOrigin = GetPlayerPos();
		if( IsInRangeTexture( tmp, m_enemyArray.at(i)->GetDrawInfo() ) ){
			isHit = true;
		}
	}
	return isHit;
}

/* ================================================ */
/**
 * @brief	各種更新関数
 */
/* ================================================ */
void EnemyManager::Update()
{
	for(uint32_t i = 0; i < m_enemyArray.size() ; ++i ){
		switch( m_enemyArray.at(i)->GetState() ){
		default:
		case EnemyBase::ENEMY_MAX:
			DEBUG_ASSERT( 0, "敵のステータスが無効");
			return;

		case EnemyBase::ENEMY_INIT:
			if( m_enemyArray.at(i)->Init() ){
				m_enemyArray.at(i)->m_enemyState = EnemyBase::ENEMY_ACTION;
			}
			break;
		case EnemyBase::ENEMY_ACTION:
			m_enemyArray.at(i)->UpdateEnemy();
			break;
		}
	}
	
	// 削除リストにいる敵を削除
	auto it = m_delEnemyArray.begin();
	for( uint32_t i = 0; it != m_delEnemyArray.end() ; ++i ){
		EnemyBase *pTmp = ( *it );
		it = m_delEnemyArray.erase( it );
		SAFE_DELETE( pTmp );
	}
}

void EnemyManager::CollisionUpdate()
{
	// 敵がプレイヤーに当たったかチェック
	GamePlayer *pPlayer = GameRegister::GetInstance()->UpdatePlayer();
	if( !pPlayer ){
		return ;
	}

	bool isHit = CheckCollisionToPlayer( pPlayer );
	if( isHit ){
		Common::CMN_EVENT hitEvent;
		hitEvent.Init();
		hitEvent.m_event		= Common::EVENT_HIT_ENEMY;
		pPlayer->AddEvent( hitEvent );
	}
}

void EnemyManager::DrawUpdate()
{
	for(uint32_t i = 0; i < m_enemyArray.size() ; ++i ){

		// 初期化中だったら無視
		if( m_enemyArray.at(i)->GetState() == EnemyBase::ENEMY_INIT ){
			continue;
		}

		m_enemyArray.at(i)->DrawEnemy();
	}
}
