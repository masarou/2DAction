/* ====================================================================== */
/**
 * @brief  敵AIの通常クラス(プレイヤー未発見)
 *
 * @note
 *		デフォルトAI
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

		// 発見「!」マーク生成
		const TEX_INIT_INFO &info = TextureResourceManager::GetInstance()->GetLoadTextureInfo( enemyInfo.m_fileName.c_str() );
		math::Vector2 dispPos = enemyInfo.m_posOrigin;
		dispPos.y -= static_cast<float>( (info.m_sizeHeight/2) );
		GameEffect::CreateEffect( GameEffect::EFFECT_EXCLAMATION, dispPos );

		// プレイヤー発見!!!
		if( GetEnemyKind() == Common::ENEMY_KIND_AAA
			|| GetEnemyKind() == Common::ENEMY_KIND_SLIME_KING ){
			DEBUG_PRINT("【プレイヤー発見! ステータスをタックルに変更】\n");
			ChangeEnemyAI( Common::AI_MOVE_PLAYER );
		}
		else if( GetEnemyKind() == Common::ENEMY_KIND_BBB ){			
			DEBUG_PRINT("【プレイヤー発見! ステータスをShootに変更】\n");
			ChangeEnemyAI( Common::AI_SHOOTING );
		}
		else if( GetEnemyKind() == Common::ENEMY_KIND_CCC ){			
			DEBUG_PRINT("【プレイヤー発見! ステータスをDashTackleに変更】\n");
			ChangeEnemyAI( Common::AI_DASH_TACKLE );
		}
		else if( GetEnemyKind() == Common::ENEMY_KIND_BOSS ){
			if( SearchPlayer( enemyInfo, 600.0f ) ){
				DEBUG_PRINT("【プレイヤー発見! ステータスをMovingに変更】\n");
				ChangeEnemyAI( Common::AI_ATTACK_NEAR );
			}
		}
		return;
	}

	// 目標となる地点を設定
	int32_t randamValue	= Utility::GetRandamValue( 10, -10 );
	math::Vector2 eyeDir = GetEnemyEyeSight();
	math::Vector2 vec = math::GetRotateVec( eyeDir, static_cast<float>(randamValue) );
	vec.Normalize();

	math::Vector2 nextPos = enemyInfo.m_posOrigin + (vec * 2.0f);
	if( !Utility::IsMovable( GetEnemyJsonName(), nextPos ) ){
		// 壁に当たったら反対を向いてみる
		vec *= -1;
	}
	enemyInfo.m_posOrigin += vec * 1.0f;

	// アニメ更新
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
