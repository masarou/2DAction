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

void EnemyAISearch::ExecMain( TEX_DRAW_INFO &enemyInfo, ACTION_INFO &actionInfo )
{
	if( SearchPlayer( enemyInfo ) ){
		// �v���C���[����!!!
		if( GetEnemyKind() == Common::ENEMY_KIND_AAA ){
			DEBUG_PRINT("�y�v���C���[����! �X�e�[�^�X���^�b�N���ɕύX�z\n");
			ChangeEnemyAI( Common::AI_MOVE_PLAYER );
		}
		else if( GetEnemyKind() == Common::ENEMY_KIND_BBB ){			
			DEBUG_PRINT("�y�v���C���[����! �X�e�[�^�X��Shoot�ɕύX�z\n");
			ChangeEnemyAI( Common::AI_SHOOTING );
		}
		return;
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

	math::Vector2 nextPos = enemyInfo.m_posOrigin + (vec * 2.0f);
	if( Utility::GetMapHeight( nextPos ) == 0 ){
		enemyInfo.m_posOrigin += vec * 2.0f;

		//// �A�j���X�V
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
	SetEnemyEyeSight( vec );
}


bool EnemyAISearch::SearchPlayer( TEX_DRAW_INFO &enemyInfo )
{
	bool retVal = false;
	if( math::IsInRange( Utility::GetPlayerPos(), enemyInfo.m_posOrigin, 200.0f ) ){
		retVal = true;
	}
	return retVal;
}