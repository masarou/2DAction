/* ====================================================================== */
/**
 * @brief  敵AIの通常クラス(プレイヤー未発見)
 *
 * @note
 *		デフォルトAI
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

		// プレイヤー発見!!!
		if( GetEnemyKind() == Common::ENEMY_KIND_SLIME
			|| GetEnemyKind() == Common::ENEMY_KIND_SLIME_ANOTHER){
			DEBUG_PRINT("【プレイヤー発見! ステータスをタックルに変更】\n");
			ChangeEnemyAI( Common::AI_MOVE_PLAYER );
		}
		else if( GetEnemyKind() == Common::ENEMY_KIND_SLIME_KING ){
			DEBUG_PRINT("【プレイヤー発見! ステータスをタックルに変更】\n");
			ChangeEnemyAI( Common::AI_MOVE_PLAYER );
		}
		else if( GetEnemyKind() == Common::ENEMY_KIND_AHRIMAN ){			
			DEBUG_PRINT("【プレイヤー発見! ステータスをShootに変更】\n");
			ChangeEnemyAI( Common::AI_SHOOTING );
		}
		else if( GetEnemyKind() == Common::ENEMY_KIND_COW ){			
			DEBUG_PRINT("【プレイヤー発見! ステータスをDashTackleに変更】\n");
			ChangeEnemyAI( Common::AI_DASH_TACKLE );
		}
		else if( GetEnemyKind() == Common::ENEMY_KIND_RANGER ){
			if( SearchPlayer( enemyInfo, 600.0f ) ){
				DEBUG_PRINT("【プレイヤー発見! ステータスをAttackに変更】\n");
				ChangeEnemyAI( Common::AI_ATTACK_NEAR );
			}
		}

		// 発見「!」マーク生成
		const TEX_INIT_INFO &info = TextureResourceManager::GetInstance()->GetLoadTextureInfo( enemyInfo.m_fileName.c_str() );
		math::Vector2 dispPos = enemyInfo.m_posOrigin;
		dispPos.y -= static_cast<float>( (info.m_sizeHeight/2) );
		GameEffect::CreateEffect( GameEffect::EFFECT_EXCLAMATION, dispPos );
		return;
	}
	
	// 目標となる地点を設定
	math::Vector2 vec;
	for( uint32_t i = 1 ;; ++i ){
		if( i > 100 ){
			DEBUG_ASSERT( 0, "目標となる位置がない\n");
			break;
		}
		int32_t randamValue	= Utility::GetRandamValue( 10*i, -10*i );
		math::Vector2 eyeDir = GetEnemyEyeSight();
		vec = math::GetRotateVec( eyeDir, static_cast<float>(randamValue) );
		math::Vector2 nextPos = enemyInfo.m_posOrigin + (vec * 2.0f);
		if( Utility::GetMapHeight( nextPos ) == 0 ){
			break;
			//// 壁に当たったら反対を向いてみる
			//vec *= -2;
		}
	}
	vec.Normalize();
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

	// デバッグ表示用
	Utility::DrawDebugCircle( ( (vec*10.0f) + enemyInfo.m_posOrigin ), 3.0f );
}


bool EnemyAISearch::SearchPlayer( TEX_DRAW_INFO &enemyInfo, float distance )
{
	// スライムで敵に体当たり中が一定数以上いるなら状態変化なし
	EnemyManager *pEnemyManager = GameRegister::GetInstance()->UpdateManagerEnemy();
	if( GetEnemyKind() == Common::ENEMY_KIND_SLIME && pEnemyManager->CountMovePlayerAI() > 3 ){
		return false;
	}
	
	// プレイヤーが一定の範囲内がどうかチェック
	if( !math::IsInRange( Utility::GetPlayerPos(), enemyInfo.m_posOrigin, distance ) ){
		// 一定の範囲外
		return false;
	}

	return true;
}
