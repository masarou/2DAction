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
		// �v���C���[����!!!
		if( GetEnemyKind() == Common::ENEMY_KIND_AAA || GetEnemyKind() == Common::ENEMY_KIND_CCC ){
			DEBUG_PRINT("�y�v���C���[����! �X�e�[�^�X���^�b�N���ɕύX�z\n");
			ChangeEnemyAI( Common::AI_MOVE_PLAYER );
		}
		else if( GetEnemyKind() == Common::ENEMY_KIND_BBB ){			
			DEBUG_PRINT("�y�v���C���[����! �X�e�[�^�X��Shoot�ɕύX�z\n");
			ChangeEnemyAI( Common::AI_SHOOTING );
		}
		return;
	}

	if( GetEnemyKind() == Common::ENEMY_KIND_BOSS ){
		if( SearchPlayer( enemyInfo, 600.0f ) ){
			DEBUG_PRINT("�y�v���C���[����! �X�e�[�^�X��Moving�ɕύX�z\n");
			ChangeEnemyAI( Common::AI_MOVE_PLAYER );
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

	const TEX_INIT_INFO &playerTexInfo = TextureResourceManager::GetInstance()->GetLoadTextureInfo( GetEnemyJsonName().c_str() );
	math::Vector2 nextPos = enemyInfo.m_posOrigin + (vec * 2.0f);
	math::Vector2 up = nextPos;
	up.y += playerTexInfo.m_sizeHeight/2.0f;
	math::Vector2 down = nextPos;
	down.y -= playerTexInfo.m_sizeHeight/2.0f;
	math::Vector2 left = nextPos;
	left.x -= playerTexInfo.m_sizeWidth/2.0f;
	math::Vector2 right = nextPos;
	right.x += playerTexInfo.m_sizeWidth/2.0f;

	if( Utility::GetMapHeight( up ) == 0
		&& Utility::GetMapHeight( down ) == 0
		&& Utility::GetMapHeight( left ) == 0
		&& Utility::GetMapHeight( right ) == 0){
		enemyInfo.m_posOrigin += vec * 1.0f;
	}
	else{
		// �ǂɓ��������甽�΂������Ă݂�
		vec *= -1;
	}

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


bool EnemyAISearch::SearchPlayer( TEX_DRAW_INFO &enemyInfo, float distance )
{
	bool retVal = false;
	if( math::IsInRange( Utility::GetPlayerPos(), enemyInfo.m_posOrigin, distance ) ){
		retVal = true;
	}
	return retVal;
}
