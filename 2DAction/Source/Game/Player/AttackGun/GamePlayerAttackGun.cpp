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


AttackGun::AttackGun(void)
	: m_intervalTime( 0 )
{
	m_currState.Init();
}


AttackGun::~AttackGun(void)
{
}

bool AttackGun::DieMain()
{
	// �ێ����Ă���N���X�����ׂĉ��
	std::vector<Bullet*>::iterator it = m_magazine.begin();
	while(m_magazine.empty() == 0){
		Bullet *bullet = (*it);
		it = m_magazine.erase(it);
		SAFE_DELETE(bullet);
	}
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
	for( uint32_t i = 0; i < m_magazine.size() ; ++i ){
		m_magazine.at( i )->Update();
	}

	// ��������Ĉ�莞�Ԃ𒴂������͍̂폜
	for( uint32_t i = 0; i < m_magazine.size() ; ++i ){
		if( m_magazine.at( i )->GetLiveTime() >= BULLET_LIVE_TIME ){
			DeleteBullet( m_magazine.at( i )->GetUniqueNumber() );
		}
	}
}

void AttackGun::DrawUpdate()
{
	// �e�`��
	for( uint32_t i = 0; i < m_magazine.size() ; ++i ){
		m_magazine.at( i )->Draw();
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
	if( m_intervalTime == 0 ){
		static uint32_t uniqueNum = 0;
		Bullet *bul = NEW Bullet( uniqueNum, pos, vec, m_currState.m_speed );
		m_magazine.push_back( bul );
		
		// ���ˉ���炷
		SoundManager::GetInstance()->PlaySE("ShootBullet");

		// ���Ԋu�̎��Ԃ�݂���
		m_intervalTime += m_currState.m_shootInterval;
	}
}

/* ================================================ */
/**
 * @brief	�e�̍폜(��ʊO�ɏo���A�G�ɓ����������X)
 */
/* ================================================ */
void AttackGun::DeleteBullet( uint32_t uniqueNumber )
{
	std::vector<Bullet*>::iterator it = m_magazine.begin();
	for( uint32_t i = 0; i < m_magazine.size() ; ++i ){
		if( (*it)->GetUniqueNumber() == uniqueNumber ){
			Bullet *pTmp = ( *it );
			m_magazine.erase( it );
			SAFE_DELETE( pTmp );
			break;
		}
		++it;
	}
}


