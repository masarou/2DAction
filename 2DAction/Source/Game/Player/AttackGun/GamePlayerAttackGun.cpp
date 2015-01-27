/* ====================================================================== */
/**
 * @brief  �v���C���[�̍U���Ǘ��N���X
 *
 * @note
 *		
 */
/* ====================================================================== */

#include "GamePlayerAttackGun.h"
#include "System/Sound/SystemSoundManager.h"

AttackGun *AttackGun::CreateGun( const Common::OWNER_TYPE &ownerType )
{
	return NEW AttackGun( ownerType );
}

AttackGun::AttackGun( const Common::OWNER_TYPE &ownerType )
: TaskUnit("AttackGun")
, m_intervalTime( 0 )
, m_owner( ownerType )
{
	m_currState.Init();
}


AttackGun::~AttackGun(void)
{
}

bool AttackGun::DieMain()
{
	m_magazine.clear();
	return true;
}

/* ================================================ */
/**
 * @brief	�ʒu�A�`��X�V�֐�
 */
/* ================================================ */
void AttackGun::Update()
{
	// ��������Ĉ�莞�Ԃ𒴂������͍̂폜
	for( auto it = m_magazine.begin(); it != m_magazine.end() ;){
		if( (*it)->GetLiveTime() >= BULLET_LIVE_TIME ){
			it = m_magazine.erase(it);
		}
		else{
			++it;
		}
	}
	// ���̒e���˂܂ł̎��Ԃ����Z
	if( m_intervalTime > 0){
		--m_intervalTime;
	}
}

/* ================================================ */
/**
 * @brief	�e�̔���
 */
/* ================================================ */
void AttackGun::ShootBullet( math::Vector2 pos, math::Vector2 vec )
{
	static uint32_t uniqueNum = 0;
	if( m_intervalTime == 0 ){
		Bullet *bul = NEW Bullet( m_owner, uniqueNum, pos, vec, m_currState.m_speed );
		m_magazine.push_back( bul );
		
		// ���ˉ���炷
		SoundManager::GetInstance()->PlaySE("ShootBullet");

		// ���Ԋu�̎��Ԃ�݂���
		m_intervalTime += m_currState.m_shootInterval;
	}
	++uniqueNum;
}

/* ================================================ */
/**
 * @brief	�e�̍폜(��ʊO�ɏo���A�G�ɓ����������X)
 */
/* ================================================ */
std::vector<Bullet*>::iterator AttackGun::DeleteBullet( uint32_t uniqueNumber )
{
	auto it = m_magazine.begin();
	for(; it != m_magazine.end() ; ++it ){
		if( (*it)->GetUniqueNumber() == uniqueNumber ){
			Bullet *pTmp = ( *it );
			it = m_magazine.erase( it );
			SAFE_DELETE( pTmp );
			break;
		}
	}
	return it;
}


