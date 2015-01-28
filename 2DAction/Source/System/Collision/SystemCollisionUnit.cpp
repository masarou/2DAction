/* ====================================================================== */
/**
 * @brief  SystemCollisionUnit.h
 *
 * @note	当たり判定を行う2Dクラスはこれを基底として画像を表示する
 *		
 */
/* ====================================================================== */

#include "SystemCollisionUnit.h"
#include "System/Collision/SystemCollisionManager.h"


Collision2DUnit::Collision2DUnit( const char *readFile )
{
	// 描画クラスセットアップ
	m_drawTexture.Init();
	if( readFile ){
		m_drawTexture.m_pTex2D = NEW Game2DBase( readFile );
	}
	
	// 管理をするように知らせる
	CollisionManager::GetInstance()->AddUnit( this );
}


Collision2DUnit::~Collision2DUnit(void)
{
	m_drawTexture.DeleteAndInit();

	// 管理を外すように知らせる
	CollisionManager::GetInstance()->RemoveUnit( this );
}

const TEX_DRAW_INFO &Collision2DUnit::GetDrawInfo() const
{
	return m_drawTexture.m_texInfo;
}
