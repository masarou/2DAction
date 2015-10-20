/* ====================================================================== */
/**
 * @brief  �GAI�̒ʏ�N���X(�v���C���[������)
 *
 * @note
 *		�f�t�H���gAI
 */
/* ====================================================================== */

#include "Game/GameRegister.h"
#include "Game/Enemy/EnemyManager.h"

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

		// �v���C���[����!!!
		if( GetEnemyKind() == Common::ENEMY_KIND_SLIME
			|| GetEnemyKind() == Common::ENEMY_KIND_SLIME_ANOTHER){
			DEBUG_PRINT("�y�v���C���[����! �X�e�[�^�X���^�b�N���ɕύX�z\n");
			ChangeEnemyAI( Common::AI_MOVE_PLAYER );
		}
		else if( GetEnemyKind() == Common::ENEMY_KIND_SLIME_KING ){
			DEBUG_PRINT("�y�v���C���[����! �X�e�[�^�X���^�b�N���ɕύX�z\n");
			ChangeEnemyAI( Common::AI_MOVE_PLAYER );
		}
		else if( GetEnemyKind() == Common::ENEMY_KIND_AHRIMAN ){			
			DEBUG_PRINT("�y�v���C���[����! �X�e�[�^�X��Shoot�ɕύX�z\n");
			ChangeEnemyAI( Common::AI_SHOOTING );
		}
		else if( GetEnemyKind() == Common::ENEMY_KIND_COW ){			
			DEBUG_PRINT("�y�v���C���[����! �X�e�[�^�X��DashTackle�ɕύX�z\n");
			ChangeEnemyAI( Common::AI_DASH_TACKLE );
		}
		else if( GetEnemyKind() == Common::ENEMY_KIND_RANGER ){
			if( SearchPlayer( enemyInfo, 600.0f ) ){
				DEBUG_PRINT("�y�v���C���[����! �X�e�[�^�X��Attack�ɕύX�z\n");
				ChangeEnemyAI( Common::AI_ATTACK_NEAR );
			}
		}

		// �����u!�v�}�[�N����
		const TEX_INIT_INFO &info = TextureResourceManager::GetInstance()->GetLoadTextureInfo( enemyInfo.m_fileName.c_str() );
		math::Vector2 dispPos = enemyInfo.m_posOrigin;
		dispPos.y -= static_cast<float>( (info.m_sizeHeight/2) );
		GameEffect::CreateEffect( GameEffect::EFFECT_EXCLAMATION, dispPos );
		return;
	}
	
	// �ڕW�ƂȂ�n�_��ݒ�
	math::Vector2 vec;
	for( uint32_t i = 1 ;; ++i ){
		if( i > 100 ){
			DEBUG_ASSERT( 0, "�ڕW�ƂȂ�ʒu���Ȃ�\n");
			break;
		}
		int32_t randamValue	= Utility::GetRandamValue( 10*i, -10*i );
		math::Vector2 eyeDir = GetEnemyEyeSight();
		vec = math::GetRotateVec( eyeDir, static_cast<float>(randamValue) );
		math::Vector2 nextPos = enemyInfo.m_posOrigin + (vec * 2.0f);
		if( Utility::GetMapHeight( nextPos ) == 0 ){
			break;
			//// �ǂɓ��������甽�΂������Ă݂�
			//vec *= -2;
		}
	}
	vec.Normalize();
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

	// �f�o�b�O�\���p
	Utility::DrawDebugCircle( ( (vec*10.0f) + enemyInfo.m_posOrigin ), 3.0f );
}


bool EnemyAISearch::SearchPlayer( TEX_DRAW_INFO &enemyInfo, float distance )
{
	// �X���C���œG�ɑ̓����蒆����萔�ȏア��Ȃ��ԕω��Ȃ�
	EnemyManager *pEnemyManager = GameRegister::GetInstance()->UpdateManagerEnemy();
	if( GetEnemyKind() == Common::ENEMY_KIND_SLIME && pEnemyManager->CountMovePlayerAI() > 3 ){
		return false;
	}
	
	// �v���C���[�����͈͓̔����ǂ����`�F�b�N
	if( !math::IsInRange( Utility::GetPlayerPos(), enemyInfo.m_posOrigin, distance ) ){
		// ���͈̔͊O
		return false;
	}

	return true;
}
