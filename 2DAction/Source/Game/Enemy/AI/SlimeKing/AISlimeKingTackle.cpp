/* ====================================================================== */
/**
 * @brief  �X���C���L���OAI�̍U���N���X(�ߋ���)
 *
 * @note
 *		�U��AI
 */
/* ====================================================================== */

#include "AISlimeKingTackle.h"
#include "Game/GameRegister.h"
#include "Game/GameManager.h"
#include "Common/Utility/CommonGameUtility.h"

AISlimeKingTackle *AISlimeKingTackle::Create()
{
	AISlimeKingTackle *tmpAI = NEW AISlimeKingTackle();
	return tmpAI;
}

AISlimeKingTackle::AISlimeKingTackle(void)
{
}


AISlimeKingTackle::~AISlimeKingTackle(void)
{
}

bool AISlimeKingTackle::InitAI()
{
	return true;
}

void AISlimeKingTackle::ExecMain( TEX_DRAW_INFO &enemyInfo, ACTION_ARRAY &actionInfo )
{
	math::Vector2 targetVec = DEFAULT_VECTOR2;
	math::Vector2 plPos = Utility::GetPlayerPos();

	// ������ȏ�v���C���[���痣�ꂽ��Search��Ԃɖ߂�
	if( !math::IsInRange( plPos, enemyInfo.m_posOrigin, DISTANCE_TO_PLAYER_SIDE ) ){
		DEBUG_PRINT("�y�v���C���[����������! �X�e�[�^�X���T�[�`�ɕύX�z\n");
		ChangeEnemyAI( Common::AI_SEARCHING_SLIME_KING );
		return;
	}

	// ���m���œG�𐶐�
	if( Utility::GetRandamValue( 30, 0 ) == 0 ){
		for(;;){
			math::Vector2 targetPos = enemyInfo.m_posOrigin;
			math::Vector2 enemyPos = math::Vector2( 
				static_cast<float>( Utility::GetRandamValue( static_cast<uint32_t>( targetPos.x + 70 ), static_cast<uint32_t>( targetPos.x - 70 ) ) ),
				static_cast<float>( Utility::GetRandamValue( static_cast<uint32_t>( targetPos.y + 70 ), static_cast<uint32_t>( targetPos.y - 70 ) ) )
				);
			if( Utility::GetMapHeight( enemyPos ) == 0){
				GameRegister::GetInstance()->UpdateManagerGame()->CreateEnemy( Common::ENEMY_KIND_SLIME, 4, true, enemyPos );
				break;
			}
		}
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

