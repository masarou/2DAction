/* ====================================================================== */
/**
 * @brief  ���X�{�XAI
 *
 * @note
 *		
 */
/* ====================================================================== */

#include "Game/GameRegister.h"
#include "Game/Enemy/EnemyManager.h"

#include "EnemyAILastBoss.h"
#include "Common/Utility/CommonGameUtility.h"
#include "System/Draw2D/SystemDraw2DResource.h"
#include "Game/Effect/GameEffect.h"

EnemyAILastBoss *EnemyAILastBoss::Create()
{
	EnemyAILastBoss *tmpAI = NEW EnemyAILastBoss();
	return tmpAI;
}

EnemyAILastBoss::EnemyAILastBoss(void)
: m_pLeftHand( NULL )
, m_pRightHand( NULL )
, m_moveBasicPos( math::Vector2() )
{
}


EnemyAILastBoss::~EnemyAILastBoss(void)
{
}

bool EnemyAILastBoss::InitAI()
{
	m_pRightHand	= LastBossRight::Create( "EnemyLastBossRight.json", math::Vector2() );
	m_pLeftHand		= LastBossLeft::Create( "EnemyLastBossLeft.json", math::Vector2() );
	return true;
}

void EnemyAILastBoss::ExecMain( TEX_DRAW_INFO &enemyInfo, ACTION_ARRAY &actionInfo )
{
	// �ŏ���������{�I��m_moveBasicPos�ŏ㏑��
	// ���̌�A�ӂ�ӂ핂���Ă��镪�̐��l�𑫂��Ă��
	if( m_moveBasicPos == math::Vector2() ){
		m_moveBasicPos = enemyInfo.m_posOrigin;
	}
	else{
		enemyInfo.m_posOrigin = m_moveBasicPos;
	}

	// ���r�̍X�V(�V���ȍs���^�C�v�ݒ��A��ʒu�̍X�V)
	ExecHandsUpdate( enemyInfo );

	// �ӂ�ӂ핂���Ă��镪�̐��l
	static uint32_t counter = 0;
	enemyInfo.m_posOrigin.y += math::Sin(counter) * 10.0f;
	++counter;

}

void EnemyAILastBoss::EnemyIsDead()
{
	// ����N���X���S
	m_pRightHand->StartDie();
}




/* ================================================ */
/**
 * @brief	���r�X�V
 */
/* ================================================ */
void EnemyAILastBoss::ExecHandsUpdate( TEX_DRAW_INFO &enemyInfo )
{
	// ��_�X�V
	math::Vector2 letfPos = enemyInfo.m_posOrigin;
	letfPos += math::Vector2( 150.0f, 50.0f );
	math::Vector2 rightPos = enemyInfo.m_posOrigin;
	rightPos += math::Vector2( -150.0f, 50.0f );

	if( m_pRightHand ){
		m_pRightHand->SetBasicPos( rightPos );
	}
	if( m_pLeftHand ){
		m_pLeftHand->SetBasicPos( letfPos );
	}

}




/* ====================================================================== */
/**
 * @brief  ���X�{�X�E�荶��N���X
 *
 * @note
 *		
 */
/* ====================================================================== */
LastBossHand::LastBossHand( const std::string &readFileName, const math::Vector2 &enemyPos )
: TaskUnit("Enemy")
, Collision2DUnit( readFileName.c_str() )
, m_basicPos( math::Vector2() )
, m_currAction( ACTION_MAX )
, m_nextAction( ACTION_MAX )
{


}

LastBossHand::~LastBossHand()
{

}

void LastBossHand::SetBasicPos( math::Vector2 basicPos )
{
	m_basicPos = basicPos;
}

void LastBossHand::Update()
{
	TEX_DRAW_INFO &drawInfo = m_drawTexture.m_pTex2D->UpdateDrawInfo();
	drawInfo.m_posOrigin = m_basicPos;
}


void LastBossHand::DrawUpdate()
{
	if( m_drawTexture.m_pTex2D ){
		m_drawTexture.m_pTex2D->DrawUpdate2D();
	}
}




LastBossRight *LastBossRight::Create( const std::string &readFileName, const math::Vector2 &enemyPos )
{
	return NEW LastBossRight( readFileName, enemyPos );
}

LastBossRight::LastBossRight( const std::string &readFileName, const math::Vector2 &enemyPos )
: LastBossHand( readFileName, enemyPos )
{

}

LastBossRight::~LastBossRight()
{
}


LastBossLeft *LastBossLeft::Create( const std::string &readFileName, const math::Vector2 &enemyPos )
{
	return NEW LastBossLeft( readFileName, enemyPos );
}

LastBossLeft::LastBossLeft( const std::string &readFileName, const math::Vector2 &enemyPos )
: LastBossHand( readFileName, enemyPos )
{

}

LastBossLeft::~LastBossLeft()
{
}