/* ====================================================================== */
/**
 * @brief  �G�Ǘ��N���X
 *
 * @note
 *		
 */
/* ====================================================================== */

#include "EnemyRanger.h"
#include "Game/GameMap.h"
#include "Game/GameRegister.h"
#include "Flow/FlowManager.h"
#include "Flow/Process/FlowProcessBossEnemyDeath.h"

EnemyRanger *EnemyRanger::Create( const uint32_t &uniqueID, const uint32_t &enemyLevel, const math::Vector2 &enemyPos )
{
	return NEW EnemyRanger( uniqueID, enemyLevel, enemyPos );
}

EnemyRanger::EnemyRanger( const uint32_t &uniqueID, const uint32_t &enemyLevel, const math::Vector2 &enemyPos )
	: EnemyBase( "EnemyRanger.json", uniqueID, Common::ENEMY_KIND_RANGER, enemyLevel, enemyPos )
{
}

EnemyRanger::~EnemyRanger(void)
{
}

const uint32_t EnemyRanger::GetEnemyDefaultHP() const
{
	// Lv�ɂ���čő僉�C�t�ύX
	return 6000 + ( 400 * GetEnemyLevel() );
}

/* ================================================ */
/**
 * @brief	�a�����󂯂����̏���
 */
/* ================================================ */
void EnemyRanger::HitPlayreSlashing( const uint32_t &damageValue )
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
uint32_t EnemyRanger::GetSlashingDamage() const
{
	return 20 + ( 6 * GetEnemyLevel() );
}

/* ================================================ */
/**
 * @brief	���S���ɌĂ΂��
 * 			�{�X�N���X���L�����Z�b�g
 */
/* ================================================ */
void EnemyRanger::EnemyDeath()
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

	// ���X�g�X�e�[�W�Ȃ�A�C�e���𗎂Ƃ�
	if( FlowManager::GetInstance()->GetCurrentFlowKind() == Common::FLOW_STAGE10 ){
		GameManager *pGameMan = GameRegister::GetInstance()->UpdateManagerGame();
		if( pGameMan ){
			pGameMan->CreateItem( Common::ITEM_KIND_LIFE_UP, GetDrawInfo().m_posOrigin );
		}
	}
}

/* ================================================ */
/**
 * @brief	�_���[�W�y������
 */
/* ================================================ */
void EnemyRanger::ReduceDamage( Common::CMN_EVENT &eventId )
{
	float levelReduce = static_cast<float>( GetEnemyLevel()*0.02f );
	switch( eventId.m_event ){
	case Common::EVENT_HIT_BULLET_PLAYER:
		// �}�V���K���U���͌����ɂ���( 0.9~4.5 )
		eventId.m_eventValue *= 0.8f - levelReduce;
		break;
	case Common::EVENT_HIT_BLADE_PLAYER:
		// �a���̓_���[�W�𑝂₷
		eventId.m_eventValue *= 1.3f - levelReduce;
		break;
	}
}
