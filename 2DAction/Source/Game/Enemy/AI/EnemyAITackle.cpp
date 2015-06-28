/* ====================================================================== */
/**
 * @brief  �GAI�̍U���N���X(�ߋ���)
 *
 * @note
 *		�U��AI
 */
/* ====================================================================== */

#include "EnemyAITackle.h"
#include "Common/Utility/CommonGameUtility.h"

EnemyAITackle *EnemyAITackle::Create()
{
	EnemyAITackle *tmpAI = NEW EnemyAITackle();
	return tmpAI;
}

EnemyAITackle::EnemyAITackle(void)
{
}


EnemyAITackle::~EnemyAITackle(void)
{
}

bool EnemyAITackle::InitAI()
{
	return true;
}

void EnemyAITackle::ExecMain( TEX_DRAW_INFO &enemyInfo, ACTION_ARRAY &actionInfo )
{
	math::Vector2 targetVec = DEFAULT_VECTOR2;
	math::Vector2 plPos = Utility::GetPlayerPos();

	// ������ȏ�v���C���[���痣�ꂽ��Search��Ԃɖ߂�
	if( !math::IsInRange( plPos, enemyInfo.m_posOrigin, 600.0f ) ){
		DEBUG_PRINT("�y�v���C���[����������! �X�e�[�^�X���T�[�`�ɕύX�z\n");
		ChangeEnemyAI( Common::AI_SEARCHING );
		return;
	}

	targetVec = plPos - enemyInfo.m_posOrigin;
	targetVec.Normalize();

	math::Vector2 nextPos = enemyInfo.m_posOrigin + ( targetVec * static_cast<float>(GetEnemySPD()) );
	if( Utility::GetMapHeight( nextPos ) == 0 ){
		enemyInfo.m_posOrigin += targetVec * static_cast<float>( GetEnemySPD() );

		//// �A�j���X�V
		std::string animTag = "";
		switch( Utility::GetDirection( targetVec.x, targetVec.y ) ){
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
	SetEnemyEyeSight( targetVec );
}

