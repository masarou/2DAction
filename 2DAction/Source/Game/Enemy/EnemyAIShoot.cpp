/* ====================================================================== */
/**
 * @brief  �GAI�̍U���N���X(������)
 *
 * @note
 *		�U��AI
 */
/* ====================================================================== */

#include "EnemyAIShoot.h"

EnemyAIShoot *EnemyAIShoot::Create()
{
	EnemyAIShoot *tmpAI = NEW EnemyAIShoot();
	return tmpAI;
}

EnemyAIShoot::EnemyAIShoot(void)
{
}


EnemyAIShoot::~EnemyAIShoot(void)
{
}

void EnemyAIShoot::ExecMain( TEX_DRAW_INFO &enemyInfo, ACTION_INFO &actionInfo )
{
	const math::Vector2 playerPos = Utility::GetPlayerPos();

	// ������ȏ�v���C���[���痣�ꂽ��Search��Ԃɖ߂�
	if( !math::IsInRange( playerPos, enemyInfo.m_posOrigin, 600.0f ) ){
		DEBUG_PRINT("�y�v���C���[����������! �X�e�[�^�X���T�[�`�ɕύX�z\n");
		ChangeEnemyAI( Common::AI_SEARCHING );
		return;
	}

	// �ړ�(�v���C���[�ɂ�����x�߂Â�)
	if( !math::IsInRange( playerPos, enemyInfo.m_posOrigin, 200.0f ) ){
		math::Vector2 eyeSight = playerPos - enemyInfo.m_posOrigin;
		eyeSight.Normalize();

		math::Vector2 nextPos = enemyInfo.m_posOrigin + eyeSight * 2.0f;
		if( Utility::GetMapHeight( nextPos ) == 0 ){
			enemyInfo.m_posOrigin += eyeSight * 2.0f;
	
			// �A�j���X�V
			std::string animTag = "";
			switch( Utility::GetDirection( eyeSight.x, eyeSight.y ) ){
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
			SetEnemyEyeSight( eyeSight );
		}
	}

	// �v���C���[�Ɍ������Ēe�𔭎�
	if( Utility::GetRandamValue( 120, 0 ) == 0 ){
		actionInfo.m_pushEventArray.push_back( Common::EVENT_SHOOT_BULLET );
	}
}
