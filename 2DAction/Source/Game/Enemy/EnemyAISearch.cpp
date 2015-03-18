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
		// プレイヤー発見!!!
		if( GetEnemyKind() == Common::ENEMY_KIND_AAA ){
			DEBUG_PRINT("【プレイヤー発見! ステータスをタックルに変更】\n");
			ChangeEnemyAI( Common::AI_MOVE_PLAYER );
		}
		else if( GetEnemyKind() == Common::ENEMY_KIND_BBB ){			
			DEBUG_PRINT("【プレイヤー発見! ステータスをShootに変更】\n");
			ChangeEnemyAI( Common::AI_SHOOTING );
		}
		return;
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

		SetEnemyEyeSight( vec );
	}
}


bool EnemyAISearch::SearchPlayer( TEX_DRAW_INFO &enemyInfo )
{
	bool retVal = false;
	if( math::IsInRange( Utility::GetPlayerPos(), enemyInfo.m_posOrigin, 300.0f ) ){
		retVal = true;
	}
	return retVal;
}
