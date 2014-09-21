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
#include "EnemyManager.h"

EnemyBase::EnemyBase( std::string jsonName, uint32_t uniqueId, Common::ENEMY_KIND kind )
: m_uniqueIdOfEnemyAll( uniqueId )
, m_enemyKind( kind )
{
	m_enemy2D = NEW Game2DBase( jsonName.c_str() );
	m_enemyInfo.Init();
}

EnemyBase::~EnemyBase()
{
	SAFE_DELETE(m_enemy2D);
}

void EnemyBase::DrawEnemy()
{
	m_enemy2D->DrawUpdate2D();
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
 * @brief	�`����擾
 */
/* ================================================ */
const TEX_DRAW_INFO &EnemyBase::GetDrawInfo()
{
	return m_enemyInfo;
}

/* ================================================ */
/**
 * @brief	�C�x���g�ɑΉ������֐��Q
 */
/* ================================================ */

// �v���C���[�̒e�ɓ�������
void EnemyBase::HitPlayreBullet()
{
	--m_HP;

	if( m_HP <= 0 ){
		// ���j�G�t�F�N�g���o��
		GameEffect *effect = NEW GameEffect( GameEffect::EFFECT_BOMB, 50,50);

		// manager�ɊǗ�����O���悤�ɓ`����
		EnemyManager::GetInstance()->DeleteEnemy( GetUniqueNumber() );
	}
}