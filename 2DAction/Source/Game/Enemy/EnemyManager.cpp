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
	// �ێ����Ă���N���X�����ׂĉ��
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
	case Common::KIND_AAA:
		pEnemy = EnemyAAA::Create( currUniqueNo );
		break;
	case Common::KIND_BBB:
		pEnemy = EnemyAAA::Create( currUniqueNo );
		break;
	case Common::KIND_CCC:
		pEnemy = EnemyAAA::Create( currUniqueNo );
		break;
	case Common::KIND_DDD:
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

void EnemyManager::DeleteEnemy( const uint32_t &uniqueNumber )
{
	std::vector<EnemyBase*>::iterator it = m_enemyArray.begin();
	for( uint32_t i = 0; i < m_enemyArray.size() ; ++i ){
		if( (*it)->GetUniqueNumber() == uniqueNumber ){
			EnemyBase *pTmp = ( *it );
			m_enemyArray.erase( it );
			SAFE_DELETE( pTmp );
			break;
		}
		++it;
	}

}

/* ================================================ */
/**
 * @brief	�G�N���X�Ƃ̓����蔻��֐�
 */
/* ================================================ */
bool EnemyManager::CheckCollision( const TEX_DRAW_INFO &texInfo )
{
	bool isHit = false;
	for( uint32_t i = 0; i < m_enemyArray.size() ; ++i){
		// �ʒu���ƃe�N�X�`���T�C�Y���܂߂ē������Ă��邩�ǂ���
		if( IsInRangeTexture( texInfo, m_enemyArray.at(i)->GetDrawInfo() ) ){
			isHit = true;
			break;
		}
	}
	return isHit;
}

bool EnemyManager::CheckCollisionToBullet( Bullet *bullet )
{
	bool isHit = false;
	for( uint32_t i = 0; i < m_enemyArray.size() ; ++i){
		// �ʒu���ƃe�N�X�`���T�C�Y���܂߂ē������Ă��邩�ǂ���
		if( IsInRangeTexture( bullet->GetDrawInfo(), m_enemyArray.at(i)->GetDrawInfo() ) ){
			Common::CMN_EVENT eventInfo;
			eventInfo.m_event = Common::EVENT_HIT_BULLET;
			eventInfo.m_eventValue = m_enemyArray.at(i)->GetUniqueNumber();
			AddEvent( eventInfo );
			isHit = true;
			break;
		}
	}
	return isHit;
}

bool EnemyManager::CheckCollisionToPlayer( GamePlayer *player )
{
	bool isHit = false;
	for( uint32_t i = 0; i < m_enemyArray.size() ; ++i){
		// �ʒu���ƃe�N�X�`���T�C�Y���܂߂ē������Ă��邩�ǂ���
		TEX_DRAW_INFO tmp = player->GetDrawInfo();
		tmp.m_pos += GetPlayerOffsetPos();
		if( IsInRangeTexture( tmp, m_enemyArray.at(i)->GetDrawInfo() ) ){
			isHit = true;
		}
	}
	return isHit;
}

/* ================================================ */
/**
 * @brief	�e��X�V�֐�
 */
/* ================================================ */
void EnemyManager::Update()
{
	for(uint32_t i = 0; i < m_enemyArray.size() ; ++i ){
		m_enemyArray.at(i)->UpdateEnemy();
	}
}

void EnemyManager::CollisionUpdate()
{
	// �G���v���C���[�ɓ����������`�F�b�N
	GamePlayer *pPlayer = GameRegister::GetInstance()->GetPlayer();
	if( !pPlayer ){
		return ;
	}

	bool isHit = CheckCollisionToPlayer( pPlayer );
	if( isHit ){
		Common::CMN_EVENT hitEvent;
		hitEvent.m_event		= Common::EVENT_HIT_ENEMY;
		hitEvent.m_eventValue	= INVALID_VALUE;
		pPlayer->AddEvent( hitEvent );
	}
}

void EnemyManager::DrawUpdate()
{
	for(uint32_t i = 0; i < m_enemyArray.size() ; ++i ){
		m_enemyArray.at(i)->DrawEnemy();
	}
}

/* ================================================ */
/**
 * @brief	�h����ł̃��b�Z�[�W����
 */
/* ================================================ */
void EnemyManager::EventUpdate( const Common::CMN_EVENT &eventId )
{
	switch( eventId.m_event ){
	case Common::EVENT_HIT_BULLET:
		{
			// �e�������������Ƃ��������Ă��
			for(uint32_t i = 0; i < m_enemyArray.size() ; ++i ){
				if( m_enemyArray.at(i)->GetUniqueNumber() == eventId.m_eventValue ){
					m_enemyArray.at(i)->EventUpdate( eventId );
				}
			}
		}
		break;

	default:

		break;
	}
}