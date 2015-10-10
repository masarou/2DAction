/* ====================================================================== */
/**
 * @brief  GamePlayerAttackGun���甭�˂���鋅�N���X
 *
 * @note
 *		GamePlayerAttackGun��e�Ƃ��Ĕ��˂��ꂽ���̃N���X
 *		�ꔭ�̋ʂ̏��������A�X�s�[�h������A�ʒu��������
 *		
 */
/* ====================================================================== */

#include "Bullet.h"
#include "Game/GameRegister.h"
#include "Common/Utility/CommonGameUtility.h"
#include "Game/Enemy/EnemyManager.h"
#include "Game/Attack/GameAttackGun.h"

Bullet::Bullet( const Common::OWNER_TYPE ownerType, const math::Vector2 &pos, const math::Vector2 &vec, const uint32_t &damage, const uint32_t &speed )
: TaskUnit( "Bullet" )
, m_ownerType( ownerType )
, m_liveTime( 0 )
, m_liveTimeMax( 0 )
, m_bulletDamage( damage )
, m_bulletVec( vec )
, m_speed( speed )
{
	std::string jsonStr = ( m_ownerType == Common::OWNER_PLAYER ) ? "Bullet.json" : "BulletEnemy.json" ;
	m_drawTexture.m_pTex2D = Game2DBase::Create( jsonStr.c_str() );

	//!�����ʒu�Z�b�g
	TEX_DRAW_INFO drawInfo;
	drawInfo.Init();
	drawInfo.m_fileName = jsonStr;
	drawInfo.m_posOrigin = pos;
	m_drawTexture.m_pTex2D->SetDrawInfo( drawInfo );

	if( m_ownerType == Common::OWNER_PLAYER ){
		m_liveTimeMax = BULLET_LIVE_TIME_PLAYER;
	}
	else{
		m_liveTimeMax = BULLET_LIVE_TIME_ENEMY;
	}

}

Bullet::~Bullet(void)
{
}

bool Bullet::DieMain()
{
	return true;
}

bool Bullet::Init()
{
	return true;
}

const TEX_DRAW_INFO &Bullet::GetDrawInfo() const
{
	if( m_drawTexture.m_pTex2D == NULL ){
		DEBUG_ASSERT( 0, "�e�̕`��N���X��NULL");
	}
	return m_drawTexture.m_pTex2D->GetDrawInfo();
}

/* ================================================ */
/**
 * @brief	�e��Update�֐�
 */
/* ================================================ */
void Bullet::Update()
{
	m_drawTexture.m_pTex2D->UpdateDrawInfo().m_posOrigin += m_bulletVec * static_cast<float>(m_speed);
	++m_liveTime;

	if( m_liveTime >= m_liveTimeMax ){
		// ���������莞�Ԃ������Ȃ玩�E
		TaskStartDie();
	}

	// �����Ɋm�F����K�v�Ȃ��̂Ń}�b�v�̍�������������
	if( m_ownerType == Common::OWNER_PLAYER
		&& Utility::GetMapHeight( m_drawTexture.m_pTex2D->UpdateDrawInfo().m_posOrigin ) > 0 ){
		// �ǂȂǂɓ��������̂Ŏ��E
		TaskStartDie();
	}
}

void Bullet::DrawUpdate()
{
	// �e�`��
	m_drawTexture.m_pTex2D->DrawUpdate2D();
}

/* ================================================ */
/**
 * @brief	���N���X����̃C�x���g����
 */
/* ================================================ */
void Bullet::EventUpdate( Common::CMN_EVENT &eventId )
{
	switch( eventId.m_event ){
	default:

		break;

	case Common::EVENT_HIT_ENEMY_SLIME:	// �G�ɓ�������
	case Common::EVENT_HIT_ENEMY_SLIME_ANOTHER:
	case Common::EVENT_HIT_ENEMY_AHRIMAN:
	case Common::EVENT_HIT_ENEMY_COW:
	case Common::EVENT_HIT_ENEMY_BOSS:
	case Common::EVENT_HIT_ENEMY_SLIME_KING:
	case Common::EVENT_HIT_ENEMY_WIZARD:
	case Common::EVENT_HIT_WIZARD_CRYSTAL:
	case Common::EVENT_HIT_DRAGON:
	case Common::EVENT_HIT_BOSS:
	case Common::EVENT_HIT_BOSS_LEFT:
	case Common::EVENT_HIT_BOSS_RIGHT:
		if( m_ownerType == Common::OWNER_PLAYER ){
			m_liveTime = m_liveTimeMax;
			TaskStartDie();
		}
		break;
		
	case Common::EVENT_HIT_PLAYER:
		if( m_ownerType == Common::OWNER_ENEMY ){
			m_liveTime = m_liveTimeMax;
			TaskStartDie();
		}
		break;

	case Common::EVENT_HIT_BLADE_PLAYER:
		// �G�̒e���v���C���[�̎a���ɓ��������炩���������
		if( m_ownerType == Common::OWNER_ENEMY ){
			TaskStartDie();
		}
		break;
	}
}

/* ================================================ */
/**
 * @brief	���̃N���X�̎�ރZ�b�g
 */
/* ================================================ */
const Common::TYPE_OBJECT Bullet::GetTypeObject() const
{
	Common::TYPE_OBJECT type = Common::TYPE_BULLET_PLAYER;
	if( m_ownerType == Common::OWNER_ENEMY ){
		type = Common::TYPE_BULLET_ENEMY;
	}
	return type;
}
