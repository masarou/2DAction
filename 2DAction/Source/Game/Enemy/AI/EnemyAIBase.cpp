
/* ====================================================================== */
/**
 * @brief  敵AIのBaseクラス
 *
 * @note
 *		
 */
/* ====================================================================== */

#include "EnemyAIBase.h"
#include "Game/Enemy/EnemyBase.h"
#include "Game/Attack/GameAttackGun.h"

AttackGun *EnemyAIBase::s_pAttackGun = NULL;

EnemyAIBase::EnemyAIBase(void)
: m_enemyMine( NULL )
, m_isReady( false )
{
	if( !s_pAttackGun ){
		// 攻撃銃作成。共有+ランダムで発射なので発射のインターバルはなし
		s_pAttackGun = AttackGun::CreateGun( Common::OWNER_ENEMY );
		AttackGun::GunState &gunStatus = s_pAttackGun->UpdateGunState();
		gunStatus.m_shootInterval = 0;
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


// 敵キャラそのものに関するget,set関数
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

void EnemyAIBase::ClearAttackGun()
{
	// 共有物にNULL設定(解放はTaskManagerが勝手にやる)
	s_pAttackGun = NULL;
}

/* ================================================ */
/**
 * @brief	便利関数
 */
/* ================================================ */
// 攻撃弾生成
void EnemyAIBase::ShootBullet( const uint32_t &damage, const uint32_t &speed, const math::Vector2 &vec )
{
	if( s_pAttackGun && m_enemyMine ){
		math::Vector2 direction = vec;
		if( vec == math::Vector2() ){
			// 方向が指定されていなかったらプレイヤーに向かって飛ばす
			direction = Utility::GetPlayerPos() - m_enemyMine->GetDrawInfo().m_posOrigin;
		}
		direction.Normalize();
		s_pAttackGun->ShootBullet( m_enemyMine->GetDrawInfo().m_posOrigin, direction, damage, speed );
	}
}
