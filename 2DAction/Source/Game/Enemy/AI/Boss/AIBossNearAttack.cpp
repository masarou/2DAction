/* ====================================================================== */
/**
 * @brief  �GAI�̒ʏ�N���X(�v���C���[������)
 *
 * @note
 *		�f�t�H���gAI
 */
/* ====================================================================== */

#include "AIBossNearAttack.h"
#include "Game/Enemy/EnemyBoss.h"
#include "Common/Utility/CommonGameUtility.h"
#include "System/Draw2D/SystemDraw2DResource.h"

AIBossNearAttack *AIBossNearAttack::Create()
{
	AIBossNearAttack *tmpAI = NEW AIBossNearAttack();
	return tmpAI;
}

AIBossNearAttack::AIBossNearAttack(void)
: m_currActionTime( 0 )
, m_targetPlace( math::Vector2() )
, m_actionType( ACTION_MAX )
{
}


AIBossNearAttack::~AIBossNearAttack(void)
{
}

bool AIBossNearAttack::InitAI()
{
	//if( Utility::GetRandamValue( 1, 0 ) == 1 ){
		// �V���b�g�K��
		ChangeActionType( ACTION_SPREAD_BULLET );
	//}
	//else{
		// �a��
		ChangeActionType( ACTION_SLASHING );
	//}

	return true;
}

void AIBossNearAttack::ExecMain( TEX_DRAW_INFO &enemyInfo, ACTION_ARRAY &actionInfo )
{
	if( !math::IsInRange( enemyInfo.m_posOrigin, Utility::GetPlayerPos(), DISTANCE_TO_PLAYER_FAR ) ){
		// ��苗�����ꂽ
		if( Utility::GetRandamValue( 60, 0 ) == 0 ){
			GameEffect::CreateEffect( GameEffect::EFFECT_PRE_EXPLOSION, Utility::GetPlayerPos() );
		}
		return;
	}

	// �U�����Ȃ�s�����Ȃ�
	if( s_pAttackBlade->IsSlashingAnimPlay() ){
		return;
	}

	if( m_targetPlace != math::Vector2() ){
		// �����A�ړI�n���Z�b�g����Ă����炻���܂ňړ����Ă���ȉ��̍s���֐������s����
		math::Vector2 moveVec = m_targetPlace - enemyInfo.m_posOrigin;
		if( moveVec.GetLengthSquare() < 10.0f*10.0f ){
			// �ړ��I��
			m_targetPlace = math::Vector2();
		}
		else{
			moveVec *= 0.075f;
		}
		if( Utility::GetMapHeight( enemyInfo.m_posOrigin + moveVec ) == 0 ){
			enemyInfo.m_posOrigin += moveVec;
		}
		else{
			// �ړ��I��
			m_targetPlace = math::Vector2();
		}
	}
	else{
		// �s���֐����Z�b�g
		switch( m_actionType ){
		case ACTION_SPREAD_BULLET:
			ExecSpreadBullet( enemyInfo, actionInfo );
			break;
		case ACTION_SLASHING:
			ExecSlashing( enemyInfo, actionInfo );
			break;
		}
	}

	// �s���֐��Ď��A�I�������玟�̍s����

	// �J��Ԃ�

	// ��Ƀv���C���[�Ƃ̋����𑪂���ȏ㗣�ꂽ���ԕύX



	// �A�j���X�V
	math::Vector2 vec = GetEnemyEyeSight();
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
	SetEnemyEyeSight( vec );

	// �v���C���[�Ɍ������Ēe�𔭎�
	if( Utility::GetRandamValue( 120, 0 ) == 0 ){
		ShootBullet();
	}
}

void AIBossNearAttack::ChangeActionType( const ACTION_TYPE &nextType )
{
	m_actionType = nextType;
	m_currActionTime = GetNowCount();
}

bool AIBossNearAttack::ExecSpreadBullet( TEX_DRAW_INFO &enemyInfo, ACTION_ARRAY &actionInfo )
{
	if( Utility::GetRandamValue( 30, 0 ) == 0 ){
		// �X�v���b�h�e����
		for( uint32_t i = 0; i < 10 ; ++i ){
			// ���L�����ƃv���C���[���Ȃ����ƍ��E�}30.0�x�͈̔͂ɔ���
			math::Vector2 myPos = enemyInfo.m_posOrigin;
			math::Vector2 plPos = Utility::GetPlayerPos();

			int32_t randamValue	= Utility::GetRandamValue( 30, -30 );
			uint32_t speed		= Utility::GetRandamValue( 20, 10 );
			math::Vector2 dir = math::GetRotateVec( plPos - myPos, static_cast<float>(randamValue) );
			ShootBullet( dir, 20, speed );
		}

		if( Utility::GetRandamValue( 3, 0 ) == 0 ){
			// ���m���ōs���ω�
			ChangeActionType( ACTION_SLASHING );
		}
		else{
			// �܂������X�v���b�h�U��
			ChangeActionType( ACTION_SPREAD_BULLET );
		}
	}
	return true;
}

bool AIBossNearAttack::ExecSlashing( TEX_DRAW_INFO &enemyInfo, ACTION_ARRAY &actionInfo )
{
	// 
	uint32_t currTime = GetNowCount();

	// �����ƃv���C���[�̋����ɂ���čs���𕪂���
	if( !math::IsInRange( enemyInfo.m_posOrigin, Utility::GetPlayerPos(), 100.0f )
		&& currTime - m_currActionTime < 3000/*3sec*/ ){
		// �܂������������̂Őڋ�
		math::Vector2 moveVec	= Utility::GetPlayerPos() - enemyInfo.m_posOrigin;
		math::Vector2 addvec	= math::Vector2();
		if( moveVec.GetLengthSquare() < 10.0f*10.0f ){
			// ���ȏ�߂Â��Ă���Ȃ��C�ɏk�߂�
			addvec += moveVec;
		}
		else{
			addvec += moveVec * 0.075f;
		}
		if( Utility::GetMapHeight( enemyInfo.m_posOrigin + addvec ) == 0 ){
			enemyInfo.m_posOrigin += addvec;
		}
		SetEnemyEyeSight( moveVec );
	}
	else{
		// �a���U��
		Slashing( Slashing::TYPE_3RD );

		// �U�����I�����痣���
		// �ړ��������
		math::Vector2 nextTargetVec = GetEnemyEyeSight() * -1.0f;
		int32_t randamValue		= Utility::GetRandamValue( 160, -160 );
		int32_t awayDistance	= Utility::GetRandamValue( 300, 700 );

		// ���̖ړI�n�����������ɋ��߂�
		nextTargetVec = math::GetRotateVec( nextTargetVec, static_cast<float>(randamValue) );

		// �ړI�n�Ɏ������Z�b�g
		SetEnemyEyeSight( nextTargetVec );

		nextTargetVec.Normalize();
		nextTargetVec *= static_cast<float>(awayDistance);
		nextTargetVec += enemyInfo.m_posOrigin;

		// �ړI�n���Z�b�g
		m_targetPlace = nextTargetVec;

		// �a���̍s���I�� ���̍s����
		if( Utility::GetRandamValue( 2, 0 ) == 0 ){
			// ���m���ōs���ω�
			ChangeActionType( ACTION_SPREAD_BULLET );
		}
		else{
			// �܂������a���s����
			ChangeActionType( ACTION_SLASHING );
		}
	}

	return true;
}