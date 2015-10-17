/* ====================================================================== */
/**
 * @brief  �GAI�̍U���N���X(�_�b�V���œˌ�)
 *
 * @note
 *		�U��AI
 */
/* ====================================================================== */

#include "EnemyAIDashTackle.h"
#include "Common/Utility/CommonGameUtility.h"

#define TACKLE_ACTION_WAIT 60
#define TACKLE_ACTION_END 180
#define TACKLE_SPEED_MAX 12.0f

EnemyAIDashTackle *EnemyAIDashTackle::Create()
{
	EnemyAIDashTackle *tmpAI = NEW EnemyAIDashTackle();
	return tmpAI;
}

EnemyAIDashTackle::EnemyAIDashTackle(void)
: m_tackleState( IDLE )
, m_actionTime( 0 )
, m_actionAccelSpeed( 0.0f )
, m_tacklePower( 0.0f )
{
}


EnemyAIDashTackle::~EnemyAIDashTackle(void)
{
}

bool EnemyAIDashTackle::InitAI()
{
	return true;
}

void EnemyAIDashTackle::ExecMain( TEX_DRAW_INFO &enemyInfo, ACTION_ARRAY &actionInfo )
{
	switch( m_tackleState ){
	default:
	case IDLE:
		UpdateIdle( enemyInfo, actionInfo );
		break;

	case ACTION:
		UpdateAction( enemyInfo, actionInfo );
		break;
		
	case PRE_END:
		UpdatePreEnd( enemyInfo, actionInfo );
		break;
	}

	//// �A�j���X�V
	math::Vector2 nextEyeSight = Utility::GetPlayerPos() - enemyInfo.m_posOrigin;
	std::string animTag = "";
	switch( Utility::GetDirection( nextEyeSight.x, nextEyeSight.y ) ){
	default:
		break;
	case InputWatcher::BUTTON_UP:
		animTag = "up";
		break;
	case InputWatcher::BUTTON_DOWN:
		animTag = "down";
		break;
	case InputWatcher::BUTTON_LEFT:
		animTag = "left";
		break;
	case InputWatcher::BUTTON_RIGHT:
		animTag = "right";
		break;
	}
	if( m_tackleState == ACTION ){
		// �����A�_�b�V���A�N�V�������Ȃ��p�A�j���ɕύX
		animTag += "Dash";
	}
	SetEnemyAnim( animTag );

	// �����X�V
	SetEnemyEyeSight( nextEyeSight );
}

void EnemyAIDashTackle::UpdateIdle( TEX_DRAW_INFO &enemyInfo, ACTION_ARRAY &actionInfo )
{
	math::Vector2 plPos = Utility::GetPlayerPos();

	// ������ȏ�v���C���[���痣�ꂽ��Search��Ԃɖ߂�
	if( !math::IsInRange( plPos, enemyInfo.m_posOrigin, 600.0f ) ){
		DEBUG_PRINT("�y�v���C���[����������! �X�e�[�^�X���T�[�`�ɕύX�z\n");
		ChangeEnemyAI( Common::AI_SEARCHING );
		return;
	}

	// �K�v�Ȓl�����Z�b�g
	m_tackleState = ACTION;
	m_actionTime = 0;
	m_actionAccelSpeed = 1.0f;
	m_tacklePower = 1.0f;
	m_tackleMoveVec = math::Vector2();

}

void EnemyAIDashTackle::UpdateAction( TEX_DRAW_INFO &enemyInfo, ACTION_ARRAY &actionInfo )
{
	math::Vector2 nextEyeSight = Utility::GetPlayerPos() - enemyInfo.m_posOrigin;
	nextEyeSight.Normalize();

	if( m_actionTime < TACKLE_ACTION_WAIT ){
		// ��Ƀv���C���[�̕�����������
		m_tackleMoveVec = nextEyeSight;
	}
	else if( m_actionTime < TACKLE_ACTION_END ){
		
		// ���ۂɍs����������(nextEyeSight)�ƌ��ݐi��ł������(m_tackleMoveVec)���傫�����Ⴕ�Ă��邩�`�F�b�N
		math::Angle angle = math::GetBetweenAngle( m_tackleMoveVec, nextEyeSight );
		if( !math::IsInRange( Utility::GetPlayerPos(), enemyInfo.m_posOrigin, 250.0f ) && angle.GetDegree() > 70.0f ){
			// �傫�����Ⴕ�Ă���̂Ō��������ɓ���
			m_tackleState = PRE_END;
		}
		else{
			// ���Ȃ��̂ő��葱����X�s�[�h����薢���Ȃ炠����
			if( m_actionAccelSpeed < 12.0f ){
				m_actionAccelSpeed *= 1.1f;
			}
			else{
				m_actionAccelSpeed = TACKLE_SPEED_MAX;
			}
		}
		math::Vector2 moveRange = m_tackleMoveVec * m_actionAccelSpeed;

		// �Ō�Ɉړ��\���ǂ����̃`�F�b�N
		math::Vector2 nextPos = enemyInfo.m_posOrigin + moveRange;
		if( Utility::GetMapHeight( nextPos ) == 0 ){
			enemyInfo.m_posOrigin += moveRange;
		}
	}
	else{
		m_tackleState = IDLE;
	}

	++m_actionTime;
}

void EnemyAIDashTackle::UpdatePreEnd( TEX_DRAW_INFO &enemyInfo, ACTION_ARRAY &actionInfo )
{
	m_actionAccelSpeed *= 0.85f;
	if( m_actionAccelSpeed < 0.5f ){
		m_tackleState = IDLE;
	}

	math::Vector2 moveRange = m_tackleMoveVec * m_actionAccelSpeed;

	// �Ō�Ɉړ��\���ǂ����̃`�F�b�N
	math::Vector2 nextPos = enemyInfo.m_posOrigin + moveRange;
	if( Utility::GetMapHeight( nextPos ) == 0 ){
		enemyInfo.m_posOrigin += moveRange;
	}
}

