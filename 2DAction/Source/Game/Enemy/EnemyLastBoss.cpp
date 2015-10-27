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
#include "Game/GameRecorder.h"
#include "Flow/FlowManager.h"
#include "Flow/Process/FlowProcessBossEnemyDeath.h"

// ��ƂȂ�Œ��HP
static const uint32_t DEFAULT_HP = 10000;

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
	if( m_drawTexture.m_pTex2D ){
		m_drawTexture.m_pTex2D->UpdateDrawInfo().m_prioity = Common::PRIORITY_ABOVE_NORMAL;
	}
	return true;
}

const uint32_t LastBoss::GetEnemyDefaultHP() const
{
	return DEFAULT_HP + ( 800 * GetEnemyLevel() );
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

	// �S�N���A�t���OON
	GameRecorder::GetInstance()->SetClearAll();
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
		eventId.m_eventValue *= 1.0f - levelReduce;
		break;
	}

	// �������Ă��郆�j�[�N�����X�^�[�̐��ŕω�
	EnemyManager *pEnemyManager = GameRegister::GetInstance()->UpdateManagerEnemy();	
	// �G�̐���
	if( pEnemyManager ){
		uint32_t countEnemy = 0;
		for( uint32_t i = 0; i < NUMBEROF(Common::s_uniqueEnemyKind) ; ++i ){
			countEnemy += pEnemyManager->CountEnemy( Common::s_uniqueEnemyKind[i] );
		}
		if( countEnemy == 1 ){
			eventId.m_eventValue *= 0.5f;
		}
		else if( countEnemy >= 2 ){
			eventId.m_eventValue = 0;
			// �_���[�W�����G�t�F�N�g
			GameEffect::CreateEffect( GameEffect::EFFECT_INVALID_DAMAGE, GetDrawInfo().m_posOrigin );
			// ����SE
			SoundManager::GetInstance()->PlaySE( "invalidDamage" );
		}
	}
}

