/* ====================================================================== */
/**
 * @brief  スライムキングAIの通常クラス(プレイヤー未発見)
 *
 * @note
 *		デフォルトAI
 */
/* ====================================================================== */

#include "AISlimeKingSearching.h"
#include "Game/GameRegister.h"
#include "Game/GameManager.h"
#include "Common/Utility/CommonGameUtility.h"
#include "System/Draw2D/SystemDraw2DResource.h"

static const uint32_t ENEMY_CREATE_VALUE = 45;

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
	if( SearchPlayer( enemyInfo, DISTANCE_TO_PLAYER_SIDE ) ){
		DEBUG_PRINT("【一定距離以内なのでステータスをタックルに変更】\n");
		ChangeEnemyAI( Common::AI_MOVE_PLAYER_SLIME_KING );
		return;
	}

	// 一定確率で敵を生成
	EnemyManager *pEnemyManager = GameRegister::GetInstance()->UpdateManagerEnemy();
	if( pEnemyManager && Utility::GetRandamValue( ENEMY_CREATE_VALUE/* - ( GetEnemyLevel() * 2 )*/, 0 ) == 0 ){
		if( pEnemyManager->CountEnemy( Common::ENEMY_KIND_SLIME_ANOTHER ) < 30 ){
			for(;;){
				math::Vector2 targetPos = enemyInfo.m_posOrigin;
				math::Vector2 enemyPos = math::Vector2( 
					static_cast<float>( Utility::GetRandamValue( static_cast<uint32_t>( targetPos.x + 200 ), static_cast<uint32_t>( targetPos.x - 200 ) ) ),
					static_cast<float>( Utility::GetRandamValue( static_cast<uint32_t>( targetPos.y + 200 ), static_cast<uint32_t>( targetPos.y - 200 ) ) )
					);
				if( Utility::GetMapHeight( enemyPos ) == 0){
					GameRegister::GetInstance()->UpdateManagerGame()->CreateEnemy( Common::ENEMY_KIND_SLIME_ANOTHER, GetEnemyLevel(), true, enemyPos );
					break;
				}
			}
		}
	}

	// 目標となる地点を設定
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

	// 移動可能かチェック
	if( Utility::GetMapHeight( enemyInfo.m_posOrigin + (vec * static_cast<float>( GetEnemySPD() ) ) ) != 0 ){
		// 移動不可なら反対を向いてみる
		vec *= -1;
	}
	enemyInfo.m_posOrigin += vec * GetEnemySPD();

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
}


bool AISlimeKingSearching::SearchPlayer( TEX_DRAW_INFO &enemyInfo, float distance )
{
	bool retVal = false;
	if( math::IsInRange( Utility::GetPlayerPos(), enemyInfo.m_posOrigin, distance ) ){
		retVal = true;
	}
	return retVal;
}
