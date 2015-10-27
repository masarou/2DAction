/* ====================================================================== */
/**
 * @brief  �GAI�̒ʏ�N���X(�v���C���[������)
 *
 * @note
 *		�f�t�H���gAI
 */
/* ====================================================================== */

#include "Game/GameRegister.h"
#include "Game/Enemy/EnemyManager.h"
#include "Game/Enemy/EnemyBase.h"
#include "Game/Enemy/EnemyWizard.h"

#include "EnemyAIDragon.h"
#include "Game/Effect/GameEffect.h"
#include "Common/Utility/CommonGameUtility.h"
#include "System/Sound/SystemSoundManager.h"
#include "System/Draw2D/SystemDraw2DResource.h"
#include "System/SystemFPSManager.h"

AIDragon *AIDragon::Create()
{
	AIDragon *tmpAI = NEW AIDragon();
	return tmpAI;
}

AIDragon::AIDragon(void)
: m_movingPos( math::Vector2() )
, m_currAction( ACTION_MAX )
, m_nextAction( ACTION_MAX )
, m_waitCounter( 0 )
{
}


AIDragon::~AIDragon(void)
{
}

bool AIDragon::InitAI()
{
	m_currAction = ACTION_FIRE_BOMB;
	m_nextAction = ACTION_MAX;

	// �ŏ��̑҂����ԃZ�b�g
	m_waitCounter = ACTION_INTERVAL_SHORT;
	return true;
}

void AIDragon::ExecMain( TEX_DRAW_INFO &enemyInfo, ACTION_ARRAY &actionInfo )
{
	// �ȉ��A�U���֐�
	if( m_waitCounter > 0 ){
		--m_waitCounter;
		return;
	}

	bool isFinish = false;
	switch( m_currAction ){
	default:
	case ACTION_FIRE_BOMB:
		isFinish = ActionShootFireBall( enemyInfo );
		break;
	case ACTION_MOVE_AWAY:
		isFinish = ActionAwayFromPlayer( enemyInfo );
		break;
	case ACTION_LARGE_EXPLOSION:
		isFinish = ActionFireWall( enemyInfo );
		break;
	}

	// �R���[�`�����Z�b�g
	if( isFinish ){
		coroutine tmp;
		m_coro = tmp;

		// ���̃A�N�V�������Z�b�g����Ă���ΕύX
		if( m_nextAction != ACTION_MAX ){
			m_currAction = m_nextAction;
			m_nextAction = ACTION_MAX;
		}
	}

	// �A�j���X�V
	std::string animTag = "";
	math::Vector2 vec = Utility::GetPlayerPos() - enemyInfo.m_posOrigin;
	vec.Normalize();
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
	SetEnemyEyeSight( vec );

	// �ړ�
	enemyInfo.m_posOrigin += vec * static_cast<float>( GetEnemySPD() );
}

/* ================================================ */
/**
 * @brief	�G�N���X�����̃N���X�����������C�x���g
 */
/* ================================================ */
void AIDragon::EnemyRecievedEvent( const Common::CMN_EVENT &eventInfo )
{
	if( GetEnemeyMine()->GetStatus() != TaskUnit::TASK_ALIVE ){
		return;
	}

	switch( eventInfo.m_event ){
	case Common::EVENT_HIT_BULLET_PLAYER:	// Player�̒e�ɓ�������
		break;

	case Common::EVENT_HIT_BLADE_PLAYER:	// Player�̎a���ɓ�������
		SetInterruptAction( ACTION_MOVE_AWAY );
		break;
	}
}

/* ================================================ */
/**
 * @brief	���̍s����`�擾
 */
/* ================================================ */
AIDragon::ACTION_TYPE AIDragon::GetRandamNextAction()
{
	uint32_t type = Utility::GetRandamValue( ACTION_MAX, 0 );
	return static_cast<ACTION_TYPE>( type );
}

/* ================================================ */
/**
 * @brief	�s���^�C�v�ύX
 */
/* ================================================ */
void AIDragon::ChangeActionType( const ACTION_TYPE &nextAction )
{
	if( m_currAction == ACTION_MAX ){
		m_currAction = nextAction;
	}
	else{
		m_nextAction = nextAction;
	}
}
void AIDragon::SetInterruptAction( const ACTION_TYPE &interruptAction )
{
	m_waitCounter = 0;
	m_currAction = interruptAction;
}


/* ================================================ */
/**
 * @brief	�΂̋ʔ���
 */
/* ================================================ */
bool AIDragon::ActionShootFireBall( TEX_DRAW_INFO &enemyInfo, const bool &onlyAction )
{
	math::Vector2 vec = Utility::GetPlayerPos() - enemyInfo.m_posOrigin;
	vec.Normalize();

	// �΂̋ʃN���X�쐬 + SE�Đ�
	DragonFireBomb::Create( enemyInfo.m_posOrigin, vec, 20 + ( GetEnemyLevel() * 3 ) );
	SoundManager::GetInstance()->PlaySE("FireBall");

	if( !onlyAction ){
		// ���̍s���Z�b�g
		m_waitCounter = ACTION_INTERVAL;
		ChangeActionType( GetRandamNextAction() );
	}

	return true;
}

/* ================================================ */
/**
 * @brief	�v���C���[���狗������鎞�̖ړI�n�ݒ�
 */
/* ================================================ */
bool AIDragon::SetRunAwayPosFromPlayer( TEX_DRAW_INFO &enemyInfo )
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

/* ================================================ */
/**
 * @brief	�ݒ肳�ꂽ�ړI�n�Ɉړ� true�œ��B
 */
/* ================================================ */
bool AIDragon::DashMove( TEX_DRAW_INFO &enemyInfo, const math::Vector2 &targetPos )
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
			addvec += moveVec * 0.02f;
		}

		// ��葬�x�𒴂��Ă����瑬�x��������
		if( addvec.GetLengthSquare() > 3.0f*3.0f ){
			addvec.Normalize();
			addvec *= 3.0f*3.0f;
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

	// �ړ����Ȃ���Ή��e��f��
	if( FpsManager::GetUpdateCounter() % 30 == 0 ){
		ActionShootFireBall( enemyInfo, /*bool onlyAction=*/true );
	}

	return retVal;
}







DragonFireBomb *DragonFireBomb::Create( const math::Vector2 &pos, const math::Vector2 &vec, const uint32_t &damage )
{
	return NEW DragonFireBomb( pos, vec, damage );
}

DragonFireBomb::DragonFireBomb( const math::Vector2 &pos, const math::Vector2 &vec, const uint32_t &damage )
: TaskUnit( "Fire" )
, m_liveTime( 0 )
, m_liveTimeMax( 0 )
, m_vec( vec )
, m_damage( damage )
{
	// �`��N���X�̏���
	std::string jsonStr = "FireBall.json";
	m_drawTexture.m_pTex2D = Game2DBase::Create( jsonStr.c_str() );

	//!�����ʒu�Z�b�g
	TEX_DRAW_INFO drawInfo;
	drawInfo.Init();
	drawInfo.m_fileName = jsonStr;
	drawInfo.m_posOrigin = pos;
	drawInfo.m_rot = pos.GetAngle( Utility::GetPlayerPos() );
	m_drawTexture.m_pTex2D->SetDrawInfo( drawInfo );

	m_liveTimeMax = 300;
}

DragonFireBomb::~DragonFireBomb()
{


}

bool DragonFireBomb::DieMain()
{
	return true;
}

bool DragonFireBomb::Init()
{
	return true;
}

/* ================================================ */
/**
 * @brief	�e��Update�֐�
 */
/* ================================================ */
void DragonFireBomb::Update()
{
	std::string currAnim = "move";
	m_drawTexture.m_pTex2D->UpdateDrawInfo().m_posOrigin += m_vec * 12.0f;
	++m_liveTime;

	if( m_liveTime >= m_liveTimeMax ){
		// ���������莞�Ԃ������Ȃ玩�E
		TaskStartDie();
	}
}

void DragonFireBomb::DrawUpdate()
{
	// �e�`��
	m_drawTexture.m_pTex2D->DrawUpdate2D();
}

/* ================================================ */
/**
 * @brief	���N���X����̃C�x���g����
 */
/* ================================================ */
void DragonFireBomb::EventUpdate( Common::CMN_EVENT &eventId )
{
	switch( eventId.m_event ){
	default:

		break;
		
	case Common::EVENT_HIT_PLAYER:
		// ���̃N���X�͎��S + ���G�t�F�N�g���쐬
		m_liveTime = m_liveTimeMax;
		TaskStartDie();
		GameEffectWithCollision::CreateEffect( Common::OWNER_ENEMY, GameEffectWithCollision::EFFECT_FIRE, Utility::GetPlayerPos() );
		break;

	case Common::EVENT_HIT_BLADE_PLAYER:
		break;
	}
}

const Common::TYPE_OBJECT DragonFireBomb::GetTypeObject() const
{
	return Common::TYPE_FIRE_BALL;
}


