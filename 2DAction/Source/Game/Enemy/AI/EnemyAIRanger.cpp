/* ====================================================================== */
/**
 * @brief RangerAI�N���X
 *
 * @note
 *		�f�t�H���gAI
 */
/* ====================================================================== */

#include "EnemyAIRanger.h"
#include "Game/Enemy/EnemyRanger.h"
#include "Common/Utility/CommonGameUtility.h"
#include "System/Draw2D/SystemDraw2DResource.h"

AIRanger *AIRanger::Create()
{
	AIRanger *tmpAI = NEW AIRanger();
	return tmpAI;
}

AIRanger::AIRanger(void)
: m_actionIntervalTime( 0 )
, m_currActionType( ACTION_MAX )
, m_nextActionType( ACTION_MAX )
, m_currActionTime( 0 )
, m_movingPos( math::Vector2() )
{
}


AIRanger::~AIRanger(void)
{
}

bool AIRanger::InitAI()
{
	m_actionIntervalTime = ACTION_INTERVAL_SHORT;

	// �a��
	ChangeActionType( ACTION_SLASHING );

	return true;
}

void AIRanger::ExecMain( TEX_DRAW_INFO &enemyInfo, ACTION_ARRAY &actionInfo )
{
	// �U�����Ȃ�s�����Ȃ�
	if( s_pAttackBlade->IsSlashingAnimPlay() ){
		return;
	}

	// �s������Ȃ̂łȂɂ����Ȃ�
	if( m_actionIntervalTime > 0 ){
		--m_actionIntervalTime;
		return;
	}

	bool isActionEnd = true;
	switch( m_currActionType ){
	case ACTION_SPREAD_BULLET:
		isActionEnd = ExecSpreadBullet( enemyInfo );
		break;
	case ACTION_SLASHING:
		isActionEnd = ExecSlashing( enemyInfo );
		break;
	case ACTION_MOVE:
		isActionEnd = ExecMoving( enemyInfo );
		break;
	}

	// �R���[�`�����Z�b�g
	if( isActionEnd ){
		coroutine tmp;
		m_coro = tmp;

		// ���̃A�N�V�������Z�b�g����Ă���ΕύX
		if( m_nextActionType != ACTION_MAX ){
			m_currActionType = m_nextActionType;
			m_nextActionType = ACTION_MAX;
		}
	}

	// �A�j���X�V
	math::Vector2 vec = Utility::GetPlayerPos() - enemyInfo.m_posOrigin;
	std::string animTag = "";
	switch( Utility::GetDirection( vec.x, vec.y ) ){
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
	SetEnemyAnim( animTag );
}

void AIRanger::ChangeActionType( const ACTION_TYPE &nextType )
{
	if( m_currActionType == ACTION_MAX ){
		m_currActionType = nextType;
	}
	else{
		m_nextActionType = nextType;
	}
}

AIRanger::ACTION_TYPE AIRanger::GetNextAction()
{
	ACTION_TYPE type;
	
	for(;;){
		// �X�v���b�h�̏ꍇ����ɋ߂����Ȃ����`�F�b�N
		type = static_cast< ACTION_TYPE >( Utility::GetRandamValue( static_cast<uint32_t>(ACTION_MAX), 0 ) );
		if( type == ACTION_SPREAD_BULLET && math::IsInRange( Utility::GetPlayerPos(), GetEnemeyMine()->GetDrawInfo().m_posOrigin, 300.0f ) ){
			continue;
		}
		else{
			break;
		}
	}
	return type;
}

bool AIRanger::ExecSpreadBullet( TEX_DRAW_INFO &enemyInfo )
{
	// �X�v���b�h�e����
	for( uint32_t i = 0; i < 8 ; ++i ){
		// ���L�����ƃv���C���[���Ȃ����ƍ��E�}25.0�x�͈̔͂ɔ���
		math::Vector2 myPos = enemyInfo.m_posOrigin;
		math::Vector2 plPos = Utility::GetPlayerPos();

		int32_t randamValue	= Utility::GetRandamValue( 25, -25 );
		uint32_t speed		= Utility::GetRandamValue( 18, 13 );
		math::Vector2 dir = math::GetRotateVec( plPos - myPos, static_cast<float>(randamValue) );
		ShootBullet( enemyInfo.m_posOrigin, dir, 7*GetEnemyLevel(), speed );
	}

	// ���̍s���Z�b�g
	ChangeActionType( GetNextAction() );

	m_actionIntervalTime = Utility::GetRandamValue( ACTION_INTERVAL, ACTION_INTERVAL_SHORT );
	return true;
}

bool AIRanger::DashMove( TEX_DRAW_INFO &enemyInfo, const math::Vector2 &targetPos )
{
	bool retVal = false;
	if( !math::IsInRange( enemyInfo.m_posOrigin, targetPos, 100.0f ) ){
		// �܂������������̂Őڋ�
		math::Vector2 moveVec	= targetPos - enemyInfo.m_posOrigin;
		math::Vector2 addvec	= math::Vector2();
		if( moveVec.GetLengthSquare() < 10.0f*10.0f ){
			// ���ȏ�߂Â��Ă���Ȃ��C�ɏk�߂�
			addvec += moveVec;
		}
		else{
			addvec += moveVec * 0.055f;
		}

		// ��葬�x�𒴂��Ă����瑬�x��������
		if( addvec.GetLengthSquare() > 5.0f*5.0f ){
			addvec.Normalize();
			addvec *= 5.0f*5.0f;
		}

		if( Utility::GetMapHeight( enemyInfo.m_posOrigin + addvec ) == 0 ){
			enemyInfo.m_posOrigin += addvec;
			SetEnemyEyeSight( moveVec );
		}
		else{
			// �ړ��悪�i�s�o���Ȃ��Ȃ玟�̃A�N�V������
			retVal = true;
		}
	}
	else{
		retVal = true;
	}
	return retVal;
}

bool AIRanger::SetRunAwayPosFromPlayer( TEX_DRAW_INFO &enemyInfo )
{
	// �ړ��������
	math::Vector2 nextTargetVec = GetEnemyEyeSight() * -1.0f;
	for( uint32_t i = 1;;++i ){
		if( i > 100 ){
			DEBUG_ASSERT( 0, "�ڕW�ƂȂ�ʒu���Ȃ�\n");
			break;
		}
		int32_t awayAngle		= 0;
		if( i < 10 ){
			awayAngle = Utility::GetRandamValue( 60, 60 );
		}
		else{
			awayAngle = Utility::GetRandamValue( i*5, i*-5 );
		}
		int32_t awayDistance	= Utility::GetRandamValue( 300, 800 );
		
		// ���̖ړI�n�����߂�
		math::Vector2 moveVec		= math::GetRotateVec( nextTargetVec, static_cast<float>(awayAngle) );
		moveVec.Normalize();
		math::Vector2 candidatePos	= enemyInfo.m_posOrigin + (moveVec * static_cast<float>(awayDistance) );
		if( Utility::GetMapHeight( candidatePos ) == 0 ){
			// �ړI�n����
			nextTargetVec = candidatePos;
			break;
		}
	}

	// �ړI�n�Ɏ������Z�b�g
	SetEnemyEyeSight( nextTargetVec );

	// �ړI�n���Z�b�g
	m_movingPos = nextTargetVec;

	return true;
}

