/* ====================================================================== */
/**
 * @brief  �G�Ǘ��N���X
 *
 * @note
 *		
 */
/* ====================================================================== */

#include "EnemyBoss.h"
#include "Game/GameMap.h"
#include "Game/GameRegister.h"
#include "Flow/FlowManager.h"
#include "Flow/Process/FlowProcessBossEnemyDeath.h"

EnemyBoss *EnemyBoss::Create( const uint32_t &uniqueID, const uint32_t &enemyLevel, const math::Vector2 &enemyPos )
{
	return NEW EnemyBoss( uniqueID, enemyLevel, enemyPos );
}

EnemyBoss::EnemyBoss( const uint32_t &uniqueID, const uint32_t &enemyLevel, const math::Vector2 &enemyPos )
	: EnemyBase( "EnemyBoss.json", uniqueID, Common::ENEMY_KIND_BOSS, enemyLevel, enemyPos )
{
}

EnemyBoss::~EnemyBoss(void)
{
}

/* ================================================ */
/**
 * @brief	�a�����󂯂����̏���
 */
/* ================================================ */
void EnemyBoss::HitPlayreSlashing( const uint32_t &damageValue )
{
	// �X�^���͂Ȃ�
	SetStunTime( 10 );
	GameEffect::CreateEffect( GameEffect::EFFECT_SLASHING_HIT, m_drawTexture.m_pTex2D->GetDrawInfo().m_posOrigin );
	UpdateEnemyDamage( damageValue );
}

/* ================================================ */
/**
 * @brief	�a����^�������̃_���[�W�ʂ�Ԃ�
 */
/* ================================================ */
uint32_t EnemyBoss::GetSlashingDamage() const
{
	return 20 + ( 6 * GetEnemyLevel() );
}

/* ================================================ */
/**
 * @brief	���S���ɌĂ΂��
 * 			�{�X�N���X���L�����Z�b�g
 */
/* ================================================ */
void EnemyBoss::EnemyDeath()
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
void EnemyBoss::ReduceDamage( Common::CMN_EVENT &eventId )
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




EnemySlimeKing *EnemySlimeKing::Create( const uint32_t &uniqueID, const uint32_t &enemyLevel, const math::Vector2 &enemyPos )
{
	return NEW EnemySlimeKing( uniqueID, enemyLevel, enemyPos );
}

EnemySlimeKing::EnemySlimeKing( const uint32_t &uniqueID, const uint32_t &enemyLevel, const math::Vector2 &enemyPos )
	: EnemyBase( "EnemySlimeKing.json", uniqueID, Common::ENEMY_KIND_SLIME_KING, enemyLevel, enemyPos )
{
}

EnemySlimeKing::~EnemySlimeKing(void)
{
}

const float EnemySlimeKing::GetEnemyDefaultSPD() const
{
	if( m_pEnemyAI && m_pEnemyAI->GetAIKind() == Common::AI_MOVE_PLAYER_SLIME_KING ){
		return 3.0f;
	}
	return 1.0f;
}

void EnemySlimeKing::EnemyDeath()
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


const uint32_t EnemySlimeKing::GetEnemyDefaultHP() const
{
	return 3500 + ( 400 * GetEnemyLevel() );
}

void EnemySlimeKing::ReduceDamage( Common::CMN_EVENT &eventId )
{
	float levelReduce = static_cast<float>( GetEnemyLevel()*0.05f );
	switch( eventId.m_event ){
	case Common::EVENT_HIT_BULLET_PLAYER:
		// �}�V���K���U���͌����ɂ���( 0.9~4.5 )
		eventId.m_eventValue *= 0.9f - levelReduce;
		break;
	case Common::EVENT_HIT_BLADE_PLAYER:
		// �a���̓_���[�W�𑝂₷
		eventId.m_eventValue *= 1.2f - levelReduce;
		break;
	}
}
