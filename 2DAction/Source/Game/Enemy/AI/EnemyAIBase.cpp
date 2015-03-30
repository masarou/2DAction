
/* ====================================================================== */
/**
 * @brief  �GAI��Base�N���X
 *
 * @note
 *		
 */
/* ====================================================================== */

#include "EnemyAIBase.h"
#include "Game/Enemy/EnemyBase.h"
#include "Game/Attack/GameAttackGun.h"

AttackGun	*EnemyAIBase::s_pAttackGun		= NULL;
AttackBlade	*EnemyAIBase::s_pAttackBlade	= NULL;

EnemyAIBase::EnemyAIBase(void)
: m_enemyMine( NULL )
, m_isReady( false )
{
	if( !s_pAttackGun ){
		// �U���e�쐬�B���L+�����_���Ŕ��˂Ȃ̂Ŕ��˂̃C���^�[�o���͂Ȃ�
		s_pAttackGun = AttackGun::CreateGun( Common::OWNER_ENEMY );
		AttackGun::GunState &gunStatus = s_pAttackGun->UpdateGunState();
		gunStatus.m_shootInterval = 0;
	}
	if( !s_pAttackBlade ){
		// �a���N���X�쐬�B���L
		s_pAttackBlade = AttackBlade::CreateAttackBlade( Common::OWNER_ENEMY );
		AttackBlade::BladeState &bladeStatus = s_pAttackBlade->UpdateBladeState();
		bladeStatus.m_damage = 20;
	}
}

EnemyAIBase::~EnemyAIBase(void)
{
}

void EnemyAIBase::SetThingingEnemy( EnemyBase *enemyMine )
{
	m_enemyMine = enemyMine;
}



void EnemyAIBase::Exec( TEX_DRAW_INFO &enemyInfo, ACTION_ARRAY &actionInfo )
{
	if( !m_isReady ){
		m_isReady = InitAI();
	}
	else{
		ExecMain( enemyInfo, actionInfo );
	}
}


// �G�L�������̂��̂Ɋւ���get,set�֐�
void EnemyAIBase::SetEnemyAnim( const std::string &animTag )
{
	if( m_enemyMine && m_enemyMine->m_drawTexture.m_pTex2D ){
		m_enemyMine->m_drawTexture.m_pTex2D->SetAnim( animTag );
	}
}

std::string EnemyAIBase::GetEnemyAnim()
{
	std::string animTag = "";
	if( m_enemyMine && m_enemyMine->m_drawTexture.m_pTex2D ){
		animTag = m_enemyMine->m_drawTexture.m_pTex2D->GetPlayAnim();
	}
	return animTag;
}


void EnemyAIBase::SetEnemyEyeSight( math::Vector2 &eye )
{
	if( m_enemyMine ){
		eye.Normalize();
		m_enemyMine->m_eye = eye;
	}
}

math::Vector2 EnemyAIBase::GetEnemyEyeSight() const
{
	if( m_enemyMine ){
		return m_enemyMine->GetEnemyEyeSight();
	}
	return DEFAULT_VECTOR2;
}

void EnemyAIBase::ChangeEnemyAI( Common::ENEMY_AI nextAI )
{
	if( !m_enemyMine ){
		return;
	}

	if( m_enemyMine->m_nextAI != nextAI ){
		m_enemyMine->m_nextAI = nextAI;
	}
}

Common::ENEMY_KIND EnemyAIBase::GetEnemyKind() const
{
	if( m_enemyMine ){
		return m_enemyMine->GetKind();
	}
	return Common::ENEMY_KIND_MAX;
}

const std::string &EnemyAIBase::GetEnemyJsonName() const
{
	return m_enemyMine->GetDrawInfo().m_fileName;
}

void EnemyAIBase::ClearAttackMaterial()
{
	// ���L����NULL�ݒ�(�����TaskManager������ɂ��)
	s_pAttackGun = NULL;
	s_pAttackBlade = NULL;
}

/* ================================================ */
/**
 * @brief	�֗��֐�
 */
/* ================================================ */
// �U���e����
void EnemyAIBase::ShootBullet( const math::Vector2 &vec, const uint32_t &damage, const uint32_t &speed )
{
	if( s_pAttackGun && m_enemyMine ){
		math::Vector2 direction = vec;
		if( vec == math::Vector2() ){
			// �������w�肳��Ă��Ȃ�������v���C���[�Ɍ������Ĕ�΂�
			direction = Utility::GetPlayerPos() - m_enemyMine->GetDrawInfo().m_posOrigin;
		}
		direction.Normalize();
		s_pAttackGun->ShootBullet( m_enemyMine->GetDrawInfo().m_posOrigin, direction, damage, speed );
	}
}

void EnemyAIBase::Slashing( const Slashing::TYPE_SLASHING &type, const math::Vector2 slashDir, const math::Vector2 &vec )
{
	if( s_pAttackBlade && m_enemyMine ){
		math::Vector2 direction = vec;
		if( vec == math::Vector2() ){
			// �������w�肳��Ă��Ȃ�������v���C���[�Ɍ������Ĕ�΂�
			direction = Utility::GetPlayerPos() - m_enemyMine->GetDrawInfo().m_posOrigin;
		}
		direction.Normalize();
		s_pAttackBlade->CreateSlashing( m_enemyMine->GetDrawInfo().m_posOrigin, direction, type );
	}
}
