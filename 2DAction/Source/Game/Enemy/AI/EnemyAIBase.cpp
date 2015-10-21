
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

AttackGun	*EnemyAIBase::s_pAttackGun		= NULL;
AttackBlade	*EnemyAIBase::s_pAttackBlade	= NULL;

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
	if( !s_pAttackBlade ){
		// 斬撃クラス作成。共有
		s_pAttackBlade = AttackBlade::CreateAttackBlade( Common::OWNER_ENEMY );
		AttackBlade::BladeState &bladeStatus = s_pAttackBlade->UpdateBladeState();
		bladeStatus.m_damage = 20;
	}

	// AIの開始時間を保持しておく
	m_thisAIStartTime  = GetNowCount();
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


void EnemyAIBase::SetEnemyEyeSight( math::Vector2 eye )
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

const uint32_t EnemyAIBase::GetEnemyLevel() const
{
	if( m_enemyMine ){
		return m_enemyMine->GetEnemyLevel();
	}
	return 0;
}

const float EnemyAIBase::GetEnemySPD() const
{
	if( m_enemyMine ){
		return m_enemyMine->GetEnemyDefaultSPD();
	}
	return 0;
}

const math::Vector2 EnemyAIBase::GetEnemyPos() const
{
	if( m_enemyMine ){
		return m_enemyMine->GetDrawInfo().m_posOrigin;
	}
	return math::Vector2();
}

EnemyBase *EnemyAIBase::UpdateEnemyMine()
{
	return m_enemyMine;
}

const EnemyBase *const EnemyAIBase::GetEnemeyMine() const
{
	return m_enemyMine;
}

void EnemyAIBase::ClearAttackMaterial()
{
	// 共有物にNULL設定(解放はTaskManagerが勝手にやる)
	s_pAttackGun = NULL;
	s_pAttackBlade = NULL;
}

uint32_t EnemyAIBase::GetTimeStartThisAIBySec()
{
	uint32_t retVal = GetNowCount() - m_thisAIStartTime;
	return retVal / 1000;
}

/* ================================================ */
/**
 * @brief	便利関数
 */
/* ================================================ */
// 攻撃弾生成
void EnemyAIBase::ShootBullet( const math::Vector2 &pos, const math::Vector2 &vec, const uint32_t &damage, const uint32_t &speed )
{
	if( s_pAttackGun && m_enemyMine ){
		math::Vector2 fromPos = pos;
		math::Vector2 direction = vec;
		if( vec == math::Vector2() ){
			// 方向が指定されていなかったらプレイヤーに向かって飛ばす
			direction = Utility::GetPlayerPos() - ( pos == math::Vector2() ? m_enemyMine->GetDrawInfo().m_posOrigin : pos );
		}
		if( fromPos == math::Vector2() ){
			// 発射位置が指定されていなかったら自身の位置から発射
			fromPos = m_enemyMine->GetDrawInfo().m_posOrigin;
		}
		direction.Normalize();
		s_pAttackGun->ShootBullet( fromPos, direction, damage, speed );
	}
}

void EnemyAIBase::Slashing( const Slashing::TYPE_SLASHING &type, const uint32_t &damageValue, const math::Vector2 &vec )
{
	if( s_pAttackBlade && m_enemyMine ){
		math::Vector2 direction = vec;
		if( vec == math::Vector2() ){
			// 方向が指定されていなかったらプレイヤーに向かって飛ばす
			direction = Utility::GetPlayerPos() - m_enemyMine->GetDrawInfo().m_posOrigin;
		}
		direction.Normalize();
		s_pAttackBlade->CreateSlashing( m_enemyMine->GetDrawInfo().m_posOrigin, direction, type );
	}
}

