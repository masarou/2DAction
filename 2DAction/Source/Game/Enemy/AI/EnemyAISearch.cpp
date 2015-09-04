/* ====================================================================== */
/**
 * @brief  �GAI�̒ʏ�N���X(�v���C���[������)
 *
 * @note
 *		�f�t�H���gAI
 */
/* ====================================================================== */

#include "EnemyAISearch.h"
#include "Common/Utility/CommonGameUtility.h"
#include "System/Draw2D/SystemDraw2DResource.h"
#include "Game/Effect/GameEffect.h"

EnemyAISearch *EnemyAISearch::Create()
{
	EnemyAISearch *tmpAI = NEW EnemyAISearch();
	return tmpAI;
}

EnemyAISearch::EnemyAISearch(void)
: m_circleRadius( 30 )
, m_circleDistance( 100 )
{
}


EnemyAISearch::~EnemyAISearch(void)
{
}

bool EnemyAISearch::InitAI()
{
	return true;
}

void EnemyAISearch::ExecMain( TEX_DRAW_INFO &enemyInfo, ACTION_ARRAY &actionInfo )
{
	if( SearchPlayer( enemyInfo ) ){

		// �����u!�v�}�[�N����
		const TEX_INIT_INFO &info = TextureResourceManager::GetInstance()->GetLoadTextureInfo( enemyInfo.m_fileName.c_str() );
		math::Vector2 dispPos = enemyInfo.m_posOrigin;
		dispPos.y -= static_cast<float>( (info.m_sizeHeight/2) );
		GameEffect::CreateEffect( GameEffect::EFFECT_EXCLAMATION, dispPos );

		// �v���C���[����!!!
		if( GetEnemyKind() == Common::ENEMY_KIND_AAA
			|| GetEnemyKind() == Common::ENEMY_KIND_SLIME_KING ){
			DEBUG_PRINT("�y�v���C���[����! �X�e�[�^�X���^�b�N���ɕύX�z\n");
			ChangeEnemyAI( Common::AI_MOVE_PLAYER );
		}
		else if( GetEnemyKind() == Common::ENEMY_KIND_BBB ){			
			DEBUG_PRINT("�y�v���C���[����! �X�e�[�^�X��Shoot�ɕύX�z\n");
			ChangeEnemyAI( Common::AI_SHOOTING );
		}
		else if( GetEnemyKind() == Common::ENEMY_KIND_CCC ){			
			DEBUG_PRINT("�y�v���C���[����! �X�e�[�^�X��DashTackle�ɕύX�z\n");
			ChangeEnemyAI( Common::AI_DASH_TACKLE );
		}
		else if( GetEnemyKind() == Common::ENEMY_KIND_BOSS ){
			if( SearchPlayer( enemyInfo, 600.0f ) ){
				DEBUG_PRINT("�y�v���C���[����! �X�e�[�^�X��Moving�ɕύX�z\n");
				ChangeEnemyAI( Common::AI_ATTACK_NEAR );
			}
		}
		return;
	}

	// �ڕW�ƂȂ�n�_��ݒ�
	int32_t randamValue	= Utility::GetRandamValue( 10, -10 );
	math::Vector2 eyeDir = GetEnemyEyeSight();
	math::Vector2 vec = math::GetRotateVec( eyeDir, static_cast<float>(randamValue) );
	vec.Normalize();

	math::Vector2 nextPos = enemyInfo.m_posOrigin + (vec * 2.0f);
	if( !Utility::IsMovable( GetEnemyJsonName(), nextPos ) ){
		// �ǂɓ��������甽�΂������Ă݂�
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

	Utility::DrawDebugCircle( (vec*10.0f) + enemyInfo.m_posOrigin );

}


bool EnemyAISearch::SearchPlayer( TEX_DRAW_INFO &enemyInfo, float distance )
{
	bool retVal = false;
	if( math::IsInRange( Utility::GetPlayerPos(), enemyInfo.m_posOrigin, distance ) ){
		retVal = true;
	}
	return retVal;
}
