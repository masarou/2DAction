/* ====================================================================== */
/**
 * @brief  �G�Ǘ��N���X
 *
 * @note
 *		
 */
/* ====================================================================== */

#include "EnemyBase.h"
#include "Game/Effect/GameEffect.h"

EnemyBase::EnemyBase( std::string jsonName, uint32_t uniqueId, Common::ENEMY_KIND kind )
: m_uniqueIdOfEnemyAll( uniqueId )
, m_enemyKind( kind )
{
	m_enemy2D = NEW Game2DBase( jsonName.c_str() );
	m_enemyInfo.Init();
}

void EnemyBase::DrawUpdate()
{
	m_enemy2D->DrawUpdate2D();
}

bool EnemyBase::DieMain()
{
	SAFE_DELETE(m_enemy2D);
	return true;
}

EnemyBase::~EnemyBase(void)
{
}

/* ================================================ */
/**
 * @brief	���N���X����̃C�x���g����
 */
/* ================================================ */
void EnemyBase::EventUpdate( const Common::CMN_EVENT &eventId )
{
	switch( eventId.m_event ){
	default:

		break;

	case Common::EVENT_HIT_BULLET:	// Player�̒e�ɓ�������
		HitPlayreBullet();
		break;

	}

}

/* ================================================ */
/**
 * @brief	�C�x���g�ɑΉ������֐��Q
 */
/* ================================================ */

// �v���C���[�̒e�ɓ�������
void EnemyBase::HitPlayreBullet()
{
	// �������ʏ����ɓ����Ă���Ȃ牽�����Ȃ�
	if(IsDie()){
		return;
	}

	--m_HP;
	if( m_HP <= 0 ){
		GameEffect *effect = NEW GameEffect( GameEffect::EFFECT_BOMB, m_,50);
		TaskStartDie();
	}
}