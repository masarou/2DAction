/* ====================================================================== */
/**
 * @brief  敵管理クラス
 *
 * @note
 *		
 */
/* ====================================================================== */

#include "EnemyBoss.h"
#include "Game/GameMap.h"
#include "Game/GameRegister.h"

EnemyBoss *EnemyBoss::Create( const uint32_t &uniqueID )
{
	return NEW EnemyBoss( uniqueID );
}

EnemyBoss::EnemyBoss( const uint32_t &uniqueID )
	: EnemyBase( "EnemyBoss.json", uniqueID, Common::ENEMY_KIND_BOSS )
{
}

EnemyBoss::~EnemyBoss(void)
{
}

bool EnemyBoss::InitMain()
{
	// 初期位置セット
	for(;;){
		math::Vector2 candidatePos = Utility::GetMapRandamPos( /*allowInWindow=*/false );
		// マップ上の動ける高さなら生成
		if( Utility::GetMapHeight( candidatePos ) <= GetWalkHeight() ){
			m_drawTexture.m_pTex2D->UpdateDrawInfo().m_posOrigin = candidatePos;
			DEBUG_PRINT( "敵生成 x = %f, y = %f\n", candidatePos.x, candidatePos.y );
			break;
		}
	}
	return true;
}

/* ================================================ */
/**
 * @brief	他のクラスからのイベントコール
 */
/* ================================================ */
void EnemyBoss::EventUpdate( const Common::CMN_EVENT &eventId )
{
	EnemyBase::EventUpdate( eventId );

}