/* ====================================================================== */
/**
 * @brief  敵AIの攻撃クラス(近距離)
 *
 * @note
 *		攻撃AI
 */
/* ====================================================================== */

#include "EnemyAITackle.h"

EnemyAITackle *EnemyAITackle::Create()
{
	EnemyAITackle *tmpAI = NEW EnemyAITackle();
	return tmpAI;
}

EnemyAITackle::EnemyAITackle(void)
{
}


EnemyAITackle::~EnemyAITackle(void)
{
}

bool EnemyAITackle::InitAI()
{
	return true;
}

void EnemyAITackle::ExecMain( TEX_DRAW_INFO &enemyInfo )
{
	math::Vector2 targetVec = DEFAULT_VECTOR2;
	math::Vector2 plPos = GetPlayerPos();

	targetVec = plPos - enemyInfo.m_posOrigin;
	targetVec.Normalize();

	math::Vector2 nextPos = enemyInfo.m_posOrigin + (targetVec * 2.0f);
	if( GetMapHeight( nextPos ) == 0 ){
		enemyInfo.m_posOrigin += targetVec * 2.0f;

		//// アニメ更新
		std::string animTag = "";
		switch( GetDirection( targetVec.x, targetVec.y ) ){
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

