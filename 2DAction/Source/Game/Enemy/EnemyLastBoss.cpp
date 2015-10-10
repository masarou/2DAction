/* ====================================================================== */
/**
 * @brief  ���X�{�X�Ǘ��N���X
 *
 * @note
 *		
 */
/* ====================================================================== */

#include "EnemyLastBoss.h"
#include "Game/GameMap.h"
#include "Game/GameRegister.h"
#include "Flow/FlowManager.h"
#include "Flow/Process/FlowProcessBossEnemyDeath.h"

LastBoss *LastBoss::Create( const uint32_t &uniqueID, const uint32_t &enemyLevel, const math::Vector2 &enemyPos )
{
	return NEW LastBoss( uniqueID, enemyLevel, enemyPos );
}

LastBoss::LastBoss( const uint32_t &uniqueID, const uint32_t &enemyLevel, const math::Vector2 &enemyPos )
	: EnemyBase( "EnemyLastBoss.json", uniqueID, Common::ENEMY_KIND_LAST_BOSS, enemyLevel, enemyPos )
{
}

LastBoss::~LastBoss(void)
{
}

bool LastBoss::InitMain()
{
	return true;
}

const uint32_t LastBoss::GetEnemyDefaultHP() const
{
	return 6000 + ( 800 * GetEnemyLevel() );
}

/* ================================================ */
/**
 * @brief	�a�����󂯂����̏���
 */
/* ================================================ */
void LastBoss::HitPlayreSlashing( const uint32_t &damageValue )
{
	// �X�^���͂Ȃ�
	SetStunTime( 10 );
	GameEffect::CreateEffect( GameEffect::EFFECT_SLASHING_HIT, m_drawTexture.m_pTex2D->GetDrawInfo().m_posOrigin );
	UpdateEnemyDamage( damageValue );
}

/* ================================================ */
/**
 * @brief	���S���ɌĂ΂��
 * 			�{�X�N���X���L�����Z�b�g
 */
/* ================================================ */
void LastBoss::EnemyDeath()
{
	// ���S�����̂Ő�p���o�Đ�
	ProcessBossEnemyDeath *pDeathEffect = ProcessBossEnemyDeath::Create( m_drawTexture.m_pTex2D->GetDrawInfo().m_posOrigin );
	FlowManager *pFlowMan = FlowManager::GetInstance();
	if( pFlowMan && pDeathEffect ){
		pFlowMan->SetupSpecialEffect( pDeathEffect );
	}
	if( m_drawTexture.m_pTex2D ){
		m_drawTexture.m_pTex2D->SetAnim( "death" );
	}
}

/* ================================================ */
/**
 * @brief	�_���[�W�y������
 */
/* ================================================ */
void LastBoss::ReduceDamage( Common::CMN_EVENT &eventId )
{
	float levelReduce = static_cast<float>( GetEnemyLevel()*0.02f );
	switch( eventId.m_event ){
	case Common::EVENT_HIT_BULLET_PLAYER:
		// �}�V���K���U���͌����ɂ���( 0.9~4.5 )
		eventId.m_eventValue *= 0.7f - levelReduce;
		break;
	case Common::EVENT_HIT_BLADE_PLAYER:
		// �a���̓_���[�W�𑝂₷
		eventId.m_eventValue *= 1.3f - levelReduce;
		break;
	}
}

