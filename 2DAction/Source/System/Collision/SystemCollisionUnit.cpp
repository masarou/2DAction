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
: m_invalidCollision( false )
, m_nextUnit( NULL )
, m_prevUnit( NULL )
, m_preBelongLv( 0 )
, m_preBelongIndex( 0 )
{
	// 描画クラスセットアップ
	m_drawTexture.Init();
	if( readFile ){
		m_drawTexture.m_pTex2D = Game2DBase::Create( readFile );
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

//void Collision2DUnit::ListUpdate()
//{
//	uint32_t currBelongLv		= m_drawTexture.m_texInfo.m_belongLv;
//	uint32_t currBelongIndex	= m_drawTexture.m_texInfo.m_belongIndex;
//
//	if( currBelongLv != m_preBelongLv || currBelongIndex != m_preBelongIndex ){
//		// 新しく双方向リストに登録をし直す
//		uint32_t index = CollisionManager::GetInstance()->GetRegisterTreeIndex( m_preBelongLv, m_preBelongIndex );
//		CollisionManager::GetInstance()->RemoveUnitFromTree( index, this );
//		index = CollisionManager::GetInstance()->GetRegisterTreeIndex( this );
//		CollisionManager::GetInstance()->RegisterUnitFromTree( index, this );
//		// 更新
//		m_preBelongLv		= currBelongLv;
//		m_preBelongIndex	= currBelongIndex;
//	}
//}

const TEX_DRAW_INFO &Collision2DUnit::GetDrawInfo() const
{
	if( !m_drawTexture.m_pTex2D ){ DEBUG_ASSERT( 0, "m_drawTexture.m_pTex2D is NULL"); }
	return m_drawTexture.m_pTex2D->GetDrawInfo();
}

const uint32_t &Collision2DUnit::GetBelongLv() const{
	if( !m_drawTexture.m_pTex2D ){ DEBUG_ASSERT( 0, "m_drawTexture.m_pTex2D is NULL"); }
	return m_drawTexture.m_pTex2D->GetDrawInfo().m_belongLv;
}

const uint32_t &Collision2DUnit::GetBelongIndex() const{
	if( !m_drawTexture.m_pTex2D ){ DEBUG_ASSERT( 0, "m_drawTexture.m_pTex2D is NULL"); }
	return m_drawTexture.m_pTex2D->GetDrawInfo().m_belongIndex;
}

void Collision2DUnit::SetNextUnit( Collision2DUnit *pUnit )
{
	if( !pUnit ){
		m_nextUnit = NULL;
		return;
	}

	if( m_nextUnit ){
		Collision2DUnit *pCurrNext = m_nextUnit;
		// もともと次にいたユニットに情報セット
		pCurrNext->SetPrevUnit( pUnit );

		// 新しく入る次のユニットに情報セット
		pUnit->SetNextUnit( pCurrNext );
		pUnit->SetPrevUnit( this );
	}
	// 自分の持つ情報更新
	m_nextUnit = pUnit;
}

void Collision2DUnit::SetPrevUnit( Collision2DUnit *pUnit )
{
	if( !pUnit ){
		m_prevUnit = NULL;
		return;
	}

	if( m_prevUnit ){
		Collision2DUnit *pCurrPrev = m_prevUnit;
		// もともと後ろにいたユニットに情報セット
		pCurrPrev->SetNextUnit( pUnit );

		// 新しく入る後ろのユニットに情報セット
		pUnit->SetNextUnit( this );
		pUnit->SetPrevUnit( pCurrPrev );
	}
	m_prevUnit = pUnit;
}
