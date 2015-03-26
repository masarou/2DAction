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

// ボスが行動する際の目的地
math::Vector2 EnemyBoss::BOSS_TARGET_PLACE = math::Vector2( static_cast<float>(INVALID_VALUE), static_cast<float>(INVALID_VALUE) );

EnemyBoss *EnemyBoss::Create( const uint32_t &uniqueID )
{
	return NEW EnemyBoss( uniqueID );
}

EnemyBoss::EnemyBoss( const uint32_t &uniqueID )
	: EnemyBase( "enemyBoss.json", uniqueID, Common::ENEMY_KIND_BOSS )
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
			m_drawTexture.m_texInfo.m_posOrigin = candidatePos;
			DEBUG_PRINT( "敵生成 x = %f, y = %f\n", m_drawTexture.m_texInfo.m_posOrigin.x, m_drawTexture.m_texInfo.m_posOrigin.y );
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