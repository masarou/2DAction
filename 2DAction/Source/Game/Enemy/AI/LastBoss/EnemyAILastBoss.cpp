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

	// ��_�X�V
	math::Vector2 letfPos = GetEnemyPos();
	letfPos += math::Vector2( 150.0f, 50.0f );
	math::Vector2 rightPos = GetEnemyPos();
	rightPos += math::Vector2( -150.0f, 50.0f );

	m_pRightHand	= LastBossRight::Create( "EnemyLastBossRight.json", rightPos );
	m_pLeftHand		= LastBossLeft::Create( "EnemyLastBossLeft.json", letfPos );
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
	m_pLeftHand->StartDie();
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
, m_basicPos( enemyPos )
, m_moveTargetPos( math::Vector2() )
, m_waitCounter( 0 )
, m_currAction( ACTION_SUMMON )
, m_nextAction( ACTION_NONE )
{


}

LastBossHand::~LastBossHand()
{

}

void LastBossHand::SetBasicPos( math::Vector2 basicPos )
{
	m_basicPos = basicPos;
}

bool LastBossHand::Init()
{
	if( m_drawTexture.m_pTex2D ){
		m_drawTexture.m_pTex2D->UpdateDrawInfo().m_posOrigin = m_basicPos;
		m_drawTexture.m_pTex2D->UpdateDrawInfo().m_rot = GetRotateDefault();
		m_drawTexture.m_pTex2D->UpdateDrawInfo().m_prioity = Common::PRIORITY_ABOVE_NORMAL;
	}
	return true;
}

bool LastBossHand::ExecAction()
{
	if( m_waitCounter > 0){
		// ���̍s���҂�
		--m_waitCounter;
		return false;
	}

	bool isActionEnd = false;
	switch( m_currAction ){
	case ACTION_NONE:
		isActionEnd = true;
		
		// ��ʒu�Ɉړ�
		m_drawTexture.m_pTex2D->UpdateDrawInfo().m_posOrigin = m_basicPos;
		break;
	case ACTION_FIST:
		isActionEnd = ExecFist( m_drawTexture.m_pTex2D->UpdateDrawInfo() );
		break;
	case ACTION_GUARD:
		isActionEnd = true;//ExecGuard( m_drawTexture.m_pTex2D->UpdateDrawInfo() );
		break;
	case ACTION_SUMMON:
		isActionEnd = ExecSummonMonster( m_drawTexture.m_pTex2D->UpdateDrawInfo() );
		break;
	case ACTION_SUMMONS:
		isActionEnd = true;//ExecSummonMonster( m_drawTexture.m_pTex2D->UpdateDrawInfo() );
		break;
	}

	if( isActionEnd ){
		// �R���[�`�����Z�b�g
		coroutine tmp;
		m_coro = tmp;

		// ���̃A�N�V�������Z�b�g����Ă���ΕύX
		if( m_nextAction != ACTION_NONE ){
			m_currAction = m_nextAction;
			m_nextAction = ACTION_MAX;
		}
		else{
			m_currAction = GetNextActionKind();
		}
	}

	return isActionEnd;
}

void LastBossHand::Update()
{
	ExecAction();
}


void LastBossHand::DrawUpdate()
{
	if( m_drawTexture.m_pTex2D ){
		m_drawTexture.m_pTex2D->DrawUpdate2D();
	}
}

LastBossHand::ACTION_KIND LastBossHand::GetNextActionKind()
{
	ACTION_KIND retAction = ACTION_NONE;
	for(;;){
		retAction = static_cast<ACTION_KIND>( Utility::GetRandamValue( ACTION_MAX, ACTION_NONE ) );
		if( retAction == ACTION_SUMMON ){
			// �����X�^�[�����̎��͏������Ă��悢���ǂ����`�F�b�N
			if( IsCreateUniqueMonster() ){
				break;
			}
		}
		else{
			break;
		}
	}
	return retAction;
}


Common::ENEMY_KIND s_uniqueEnemyKind[] = {
	Common::ENEMY_KIND_WIZARD,
	Common::ENEMY_KIND_DRAGON,
	Common::ENEMY_KIND_BOSS,
};

bool LastBossHand::IsCreateUniqueMonster() const
{
	EnemyManager *pEnemyManager = GameRegister::GetInstance()->UpdateManagerEnemy();	
	// �G�̐���
	if( pEnemyManager ){
		uint32_t countEnemy = 0;
		for( uint32_t i = 0; i < NUMBEROF(s_uniqueEnemyKind) ; ++i ){
			countEnemy += pEnemyManager->CountEnemy( s_uniqueEnemyKind[i] );
		}
		if( countEnemy < 2 ){
			return true;
		}
	}
	return false;
}

Common::ENEMY_KIND LastBossHand::DecideCreateMonster()
{
	// �������郂���X�^�[������
	// ���������X�^�[�͔�����
	Common::ENEMY_KIND kind = Common::ENEMY_KIND_SLIME;
	EnemyManager *pEnemyManager = GameRegister::GetInstance()->UpdateManagerEnemy();
	if( pEnemyManager ){
		for(;;){
			kind = s_uniqueEnemyKind[ Utility::GetRandamValue( NUMBEROF( s_uniqueEnemyKind ), 0 ) ];
			// ���ɓ��������X�^�[����������Ă��邩�ǂ����`�F�b�N
			if( pEnemyManager->CountEnemy( kind ) == 0 ){
				break;
			}
		}
	}
	return kind;
}

bool LastBossHand::MoveToTargetPos( const math::Vector2 &targetPos, const float &maxSpeed, const float &rateSpeed )
{
	TEX_DRAW_INFO &drawInfo = m_drawTexture.m_pTex2D->UpdateDrawInfo();
	math::Vector2 currentPos = drawInfo.m_posOrigin;
	math::Vector2 diff = targetPos - currentPos;

	bool isMoveFinish = false;
	if( diff.GetLength() < 20.0f ){
		// ���������l�ȉ��Ȃ�ړI�n�ɓ��B
		drawInfo.m_posOrigin = targetPos;
		isMoveFinish = true;
	}
	else{
		// maxSpeed�𒴂��Ĉړ����Ȃ��悤�ɂ��Ȃ���ړ�1
		math::Vector2 move = diff * rateSpeed;
		if( move.GetLength() >= maxSpeed ){
			move.Normalize();
			move *= maxSpeed;
		}
		drawInfo.m_posOrigin += move;
	}
	return isMoveFinish;
}


// �w��̉�]�p�x�܂ň��ʂŉ�]
bool LastBossHand::RotateToTargetAngle( const math::Angle &targetDegree, const bool &forceSet )
{
	math::Angle currDegree = m_drawTexture.m_pTex2D->GetDrawInfo().m_rot;

	float diff = targetDegree.GetDegree() - currDegree.GetDegree();
	
	bool isFinish = false;
	if( math::Absf(diff) < 5.0f || forceSet ){
		m_drawTexture.m_pTex2D->UpdateDrawInfo().m_rot = targetDegree;
		isFinish = true;
	}
	else{
		m_drawTexture.m_pTex2D->UpdateDrawInfo().m_rot += diff * 0.03f;
	}
	return isFinish;
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