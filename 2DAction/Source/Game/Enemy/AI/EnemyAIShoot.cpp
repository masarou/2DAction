/* ====================================================================== */
/**
 * @brief  �GAI�̍U���N���X(������)
 *
 * @note
 *		�U��AI
 */
/* ====================================================================== */

#include "EnemyAIShoot.h"
#include "Common/Utility/CommonGameUtility.h"

EnemyAIShoot *EnemyAIShoot::Create()
{
	EnemyAIShoot *tmpAI = NEW EnemyAIShoot();
	return tmpAI;
}

EnemyAIShoot::EnemyAIShoot(void)
: m_shootInterval( 0 )
{
}


EnemyAIShoot::~EnemyAIShoot(void)
{
}

void EnemyAIShoot::ExecMain( TEX_DRAW_INFO &enemyInfo, ACTION_ARRAY &actionInfo )
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

		math::Vector2 nextPos = enemyInfo.m_posOrigin + eyeSight * static_cast<float>(GetEnemySPD());
		if( Utility::GetMapHeight( nextPos ) == 0 ){
			enemyInfo.m_posOrigin += eyeSight * static_cast<float>(GetEnemySPD());
	
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

	if( m_shootInterval > 0){
		--m_shootInterval;
	}

	if( m_shootInterval == 0
		&& Utility::GetRandamValue( 120, 0 ) == 0 ){
		// �v���C���[�Ɍ������Ēe�𔭎�
		uint32_t damage = 20 + GetEnemyLevel() * 5;
		ShootBullet( math::Vector2(), math::Vector2(), damage );
		m_shootInterval = ACTION_INTERVAL;
	}
}
