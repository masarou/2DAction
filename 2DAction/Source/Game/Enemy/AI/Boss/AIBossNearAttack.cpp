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
{
	m_targetPlace = EnemyBoss::BOSS_TARGET_PLACE;
}


AIBossNearAttack::~AIBossNearAttack(void)
{
}

bool AIBossNearAttack::InitAI()
{
	if( Utility::GetRandamValue( 1, 0 ) == 0 ){
		// �V���b�g�K��
		m_actionType = ACTION_SPREAD_BULLET;
	}
	else{
		// �a��
		m_actionType = ACTION_SLASHING;
	}

	return true;
}

void AIBossNearAttack::ExecMain( TEX_DRAW_INFO &enemyInfo, ACTION_ARRAY &actionInfo )
{
	if( !math::IsInRange( enemyInfo.m_posOrigin, Utility::GetPlayerPos(), DISTANCE_TO_PLAYER_FAR ) ){
		// ��苗�����ꂽ
		//ChangeEnemyAI( Common::AI_CREATE_ENEMY
		return;
	}

	// �s���֐����Z�b�g
	switch( m_actionType ){
	case ACTION_SPREAD_BULLET:
		ExecSpreadBullet( enemyInfo, actionInfo );
		break;
	case ACTION_SLASHING:
		ExecSlashing( enemyInfo, actionInfo );
		break;
	case ACTION_RUN_AWAY:
		ExecRunAway( enemyInfo, actionInfo );
		break;
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
		// �s���̏ڍׂ��Z�b�g
		ACTION_INFO action;
		action.m_AItype = AI_SHOOT_BULLET;
		action.m_AIInfo.Init();
		action.m_EventMessage	= Common::EVENT_SHOOT_BULLET;

		// ��قǃC�x���g���b�Z�[�W�Ƃ��ē����Ă��炤
		actionInfo.m_pushEventArray.push_back( action );
	}
}


bool AIBossNearAttack::ExecSpreadBullet( TEX_DRAW_INFO &enemyInfo, ACTION_ARRAY &actionInfo )
{
	if( math::IsInRange( enemyInfo.m_posOrigin, Utility::GetPlayerPos(), 150.0f ) ){

		// �X�v���b�h�e����
		//CreateSpreadBullet( actionInfo );
		//actionInfo.m_pushEventArray.push_back( Common::EVENT_SHOOT_BULLET );
	}
	return true;
}

bool AIBossNearAttack::ExecSlashing( TEX_DRAW_INFO &enemyInfo, ACTION_ARRAY &actionInfo )
{
	return true;
}

bool AIBossNearAttack::ExecRunAway( TEX_DRAW_INFO &enemyInfo, ACTION_ARRAY &actionInfo )
{
	return true;
}

void AIBossNearAttack::CreateSpreadBullet( ACTION_ARRAY &actionInfo )
{
	for( uint32_t i = 0; i < 10 ; ++i ){

	}
}

