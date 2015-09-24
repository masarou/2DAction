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
#include "Game/Enemy/EnemyBase.h"
#include "Game/Enemy/EnemyWizard.h"

#include "EnemyAIWizard.h"
#include "Common/Utility/CommonGameUtility.h"
#include "System/Draw2D/SystemDraw2DResource.h"
#include "Game/Effect/GameEffect.h"

AIWizard *AIWizard::Create()
{
	AIWizard *tmpAI = NEW AIWizard();
	return tmpAI;
}

AIWizard::AIWizard(void)
: m_circleRadius( 30 )
, m_circleDistance( 100 )
{
}


AIWizard::~AIWizard(void)
{
}

bool AIWizard::InitAI()
{
	return true;
}

void AIWizard::ExecMain( TEX_DRAW_INFO &enemyInfo, ACTION_ARRAY &actionInfo )
{
	if( Utility::GetRandamValue( 100, 0 ) != 0 ){
		return ;
	}
	if( GetEnemeyMine() && GetEnemeyMine()->GetKind() == Common::ENEMY_KIND_WIZARD ){
		const EnemyWizard *const pWizard = static_cast<const EnemyWizard *const>( GetEnemeyMine() );
		math::Vector2 pos = pWizard->GetCrystalPosFirst();
		ShootBullet( pos );
		pos = pWizard->GetCrystalPosSecond();
		ShootBullet( pos );
	}
	// アニメ更新
	//std::string animTag = "";
	//switch( Utility::GetDirection( vec.x, vec.y ) ){
	//default:
	//	break;
	//case InputWatcher::BUTTON_UP:
	//	animTag = "up";
	//	break;
	//case InputWatcher::BUTTON_DOWN:
	//	animTag = "down";
	//	break;
	//case InputWatcher::BUTTON_LEFT:
	//	animTag = "left";
	//	break;
	//case InputWatcher::BUTTON_RIGHT:
	//	animTag = "right";
	//	break;
	//}
	//SetEnemyAnim( animTag );
	//SetEnemyEyeSight( vec );

}
