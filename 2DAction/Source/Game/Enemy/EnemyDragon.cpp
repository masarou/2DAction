/* ====================================================================== */
/**
 * @brief  �G�Ǘ��N���X
 *
 * @note
 *		
 */
/* ====================================================================== */

#include "EnemyDragon.h"
#include "Game/GameMap.h"
#include "Game/GameRegister.h"
#include "Flow/FlowManager.h"
#include "Flow/Process/FlowProcessBossEnemyDeath.h"

EnemyDragon *EnemyDragon::Create( const uint32_t &uniqueID )
{
	return NEW EnemyDragon( uniqueID );
}

EnemyDragon::EnemyDragon( const uint32_t &uniqueID )
	: EnemyBase( "EnemyDragon.json", uniqueID, Common::ENEMY_KIND_DRAGON, 0 )
{
}

EnemyDragon::~EnemyDragon(void)
{
}


const uint32_t EnemyDragon::GetEnemyDefaultHP() const
{
	return 6000 + ( 800 * GetEnemyLevel() );
}

/* ================================================ */
/**
 * @brief	�a�����󂯂����̏���
 */
/* ================================================ */
void EnemyDragon::HitPlayreSlashing( const uint32_t &damageValue )
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
void EnemyDragon::EnemyDeath()
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
void EnemyDragon::ReduceDamage( Common::CMN_EVENT &eventId )
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
