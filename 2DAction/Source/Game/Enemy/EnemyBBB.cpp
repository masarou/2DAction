/* ====================================================================== */
/**
 * @brief  敵管理クラス
 *
 * @note
 *		
 */
/* ====================================================================== */

#include "EnemyBBB.h"
#include "Game/GameMap.h"
#include "Game/GameRegister.h"

EnemyBBB *EnemyBBB::Create( const uint32_t &uniqueID )
{
	return NEW EnemyBBB( uniqueID );
}

EnemyBBB::EnemyBBB( const uint32_t &uniqueID )
: EnemyBase( "EnemyAhriman.json", uniqueID, Common::ENEMY_KIND_BBB )
{
}

EnemyBBB::~EnemyBBB(void)
{
}

bool EnemyBBB::InitMain()
{
	// 初期位置セット
	m_drawTexture.m_pTex2D->UpdateDrawInfo().m_posOrigin = Utility::GetMapRandamPos( /*allowInWindow=*/false );
	DEBUG_PRINT( "敵生成 x = %f, y = %f\n", m_drawTexture.m_pTex2D->GetDrawInfo().m_posOrigin.x, m_drawTexture.m_pTex2D->GetDrawInfo().m_posOrigin.y );

	return true;
}

/* ================================================ */
/**
 * @brief	他のクラスからのイベントコール
 */
/* ================================================ */
void EnemyBBB::EventUpdate( const Common::CMN_EVENT &eventId )
{
	EnemyBase::EventUpdate( eventId );

}

uint32_t EnemyBBB::GetEnemyDefaultHP() const
{
	// ステージによって最大ライフ変更
	return 100 + ( 100 * ( GetEnemyLevel() - 1 ) );
}