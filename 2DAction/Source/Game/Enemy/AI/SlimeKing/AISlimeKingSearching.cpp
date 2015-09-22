/* ====================================================================== */
/**
 * @brief  �X���C���L���OAI�̒ʏ�N���X(�v���C���[������)
 *
 * @note
 *		�f�t�H���gAI
 */
/* ====================================================================== */

#include "AISlimeKingSearching.h"
#include "Game/GameRegister.h"
#include "Game/GameManager.h"
#include "Common/Utility/CommonGameUtility.h"
#include "System/Draw2D/SystemDraw2DResource.h"

AISlimeKingSearching *AISlimeKingSearching::Create()
{
	AISlimeKingSearching *tmpAI = NEW AISlimeKingSearching();
	return tmpAI;
}

AISlimeKingSearching::AISlimeKingSearching(void)
: m_circleRadius( 30 )
, m_circleDistance( 100 )
{
}


AISlimeKingSearching::~AISlimeKingSearching(void)
{
}

bool AISlimeKingSearching::InitAI()
{
	return true;
}

void AISlimeKingSearching::ExecMain( TEX_DRAW_INFO &enemyInfo, ACTION_ARRAY &actionInfo )
{
	if( SearchPlayer( enemyInfo, static_cast<float>(DISTANCE_TO_PLAYER_MIDDLE) ) ){
		// ���m���ő̓�����֕ύX
		if( SearchPlayer( enemyInfo, static_cast<float>(DISTANCE_TO_PLAYER_NEAR) ) ){
			DEBUG_PRINT("�y��苗���ȓ��Ȃ̂ŃX�e�[�^�X���^�b�N���ɕύX�z\n");
			ChangeEnemyAI( Common::AI_MOVE_PLAYER_SLIME_KING );
			return;
		}
		// ���m�� or �ܕb�o�߂ő̓�����֕ύX
		if( Utility::GetRandamValue( 240, 0 ) == 0
			|| GetSecStartThisAI() >= 5 ){
			DEBUG_PRINT("�y���m�� or �ܕb�o�߂����̂ŃX�e�[�^�X���^�b�N���ɕύX�z\n");
			ChangeEnemyAI( Common::AI_MOVE_PLAYER_SLIME_KING );
			return;
		}
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

	// �ڕW�ƂȂ�n�_��ݒ�
	math::Vector2 vec = math::Vector2( Utility::GetRandamValueFloat( 100, -100 ), Utility::GetRandamValueFloat( 100, -100 ) );
	vec.Normalize();
	vec *= static_cast<float>(m_circleRadius);

	vec += enemyInfo.m_posOrigin;

	const math::Vector2 &eye = GetEnemyEyeSight();
	if( eye != DEFAULT_VECTOR2 ){
		vec += eye * static_cast<float>(m_circleDistance);
	}

	vec -= enemyInfo.m_posOrigin;
	vec.Normalize();

	// �ړ��\���`�F�b�N
	if( Utility::GetMapHeight( enemyInfo.m_posOrigin + (vec * 2.0f) ) != 0 ){
		// �ړ��s�Ȃ甽�΂������Ă݂�
		vec *= -1;
	}
	enemyInfo.m_posOrigin += vec * 1.0f;

	// �A�j���X�V
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
}


bool AISlimeKingSearching::SearchPlayer( TEX_DRAW_INFO &enemyInfo, float distance )
{
	bool retVal = false;
	if( math::IsInRange( Utility::GetPlayerPos(), enemyInfo.m_posOrigin, distance ) ){
		retVal = true;
	}
	return retVal;
}
