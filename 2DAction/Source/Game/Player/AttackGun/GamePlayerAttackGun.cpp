/* ====================================================================== */
/**
 * @brief  �v���C���[�N���X
 *
 * @note
 *		
 */
/* ====================================================================== */

#include "GamePlayerAttackGun.h"

// �Œ�l
static const uint32_t SHOOT_INTERBAL	= 10;

AttackGun::AttackGun(void)
	: m_intervalTime( 0 )
{
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
}

void AttackGun::DrawUpdate()
{
	for( uint32_t i = 0; i < m_magazine.size() ; ++i ){
		m_magazine.at( i )->Draw();
	}

	// ��������Ĉ�莞�Ԃ𒴂������͍̂폜
	for( uint32_t i = 0; i < m_magazine.size() ; ++i ){
		if( m_magazine.at( i )->GetLiveTime() >= BULLET_LIVE_TIME ){
			DeleteBullet( m_magazine.at( i )->GetUniqueNumber() );
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
	if( m_intervalTime == 0 ){
		static uint32_t uniqueNum = 0;
		Bullet *bul = NEW Bullet( uniqueNum, pos, vec );
		m_magazine.push_back( bul );

		// ���Ԋu�̎��Ԃ�݂���
		m_intervalTime += SHOOT_INTERBAL;
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


