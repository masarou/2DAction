/* ====================================================================== */
/**
 * @brief  スライムキングAIの攻撃クラス(近距離)
 *
 * @note
 *		攻撃AI
 */
/* ====================================================================== */

#include "AISlimeKingTackle.h"
#include "Game/GameRegister.h"
#include "Game/GameManager.h"
#include "Common/Utility/CommonGameUtility.h"

static const uint32_t ENEMY_CREATE_VALUE = 45;
static const float ENEMY_CREATE_RANGE = 70.0f;

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

	// ある一定以上プレイヤーから離れたらSearch状態に戻す
	if( !math::IsInRange( plPos, enemyInfo.m_posOrigin, DISTANCE_TO_PLAYER_SIDE ) ){
		DEBUG_PRINT("【プレイヤーを見失った! ステータスをサーチに変更】\n");
		ChangeEnemyAI( Common::AI_SEARCHING_SLIME_KING );
		return;
	}

	// 一定確率で敵を生成
	EnemyManager *pEnemyManager = GameRegister::GetInstance()->UpdateManagerEnemy();
	if( pEnemyManager && Utility::GetRandamValue( ENEMY_CREATE_VALUE/* - ( GetEnemyLevel() * 2 )*/, 0 ) == 0 ){
		if( pEnemyManager->CountEnemy( Common::ENEMY_KIND_SLIME_ANOTHER ) < 25 ){
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

	targetVec = plPos - enemyInfo.m_posOrigin;
	targetVec.Normalize();

	math::Vector2 nextPos = enemyInfo.m_posOrigin + ( targetVec * static_cast<float>(GetEnemySPD()) );
	if( Utility::GetMapHeight( nextPos ) == 0 ){
		enemyInfo.m_posOrigin += targetVec * static_cast<float>( GetEnemySPD() );

		//// アニメ更新
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

