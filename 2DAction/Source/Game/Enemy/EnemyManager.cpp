/* ====================================================================== */
/**
 * @brief  �G�Ǘ��N���X
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
	// �ێ����Ă���N���X�����ׂď��O
	m_enemyArray.clear();

	return true;
}

/* ================================================ */
/**
 * @brief	�G�N���X�̒ǉ��Ə��O
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
		DEBUG_ASSERT( 0,  "�G�̎�ނ��z��O" );
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
 * @brief	�ʒu�A�`��X�V�֐�
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
	//	// �擾�����A�C�e���̐������C�x���gpush
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
 * @brief	�G�N���X�Ƃ̓����蔻��֐�
 */
/* ================================================ */
bool EnemyManager::CheckCollision( const TEX_DRAW_INFO &texInfo ) const
{
	bool isHit = false;
	for( uint32_t i = 0; i < m_enemyArray.size() ; ++i){
		
		// ���������������疳��
		if( m_enemyArray.at(i)->GetStatus() == TaskUnit::TASK_INIT ){
			continue;
		}

		// �ʒu���ƃe�N�X�`���T�C�Y���܂߂ē������Ă��邩�ǂ���
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
//		// ���������������疳��
//		if( m_enemyArray.at(i)->GetStatus() == TaskUnit::TASK_INIT ){
//			continue;
//		}
//
//		// �ʒu���ƃe�N�X�`���T�C�Y���܂߂ē������Ă��邩�ǂ���
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
//		// ���������������疳��
//		if( m_enemyArray.at(i)->GetStatus() == TaskUnit::TASK_INIT ){
//			continue;
//		}
//
//		// �ʒu���ƃe�N�X�`���T�C�Y���܂߂ē������Ă��邩�ǂ���
//		TEX_DRAW_INFO tmp = player->GetDrawInfo();
//		tmp.m_posOrigin = GetPlayerPos();
//		if( IsInRangeTexture( tmp, m_enemyArray.at(i)->GetDrawInfo() ) ){
//			isHit = true;
//		}
//	}
//	return isHit;
//}
