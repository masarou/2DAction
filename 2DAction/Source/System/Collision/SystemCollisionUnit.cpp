/* ====================================================================== */
/**
 * @brief  SystemCollisionUnit.h
 *
 * @note	�����蔻����s��2D�N���X�͂�������Ƃ��ĉ摜��\������
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
	// �`��N���X�Z�b�g�A�b�v
	m_drawTexture.Init();
	if( readFile ){
		m_drawTexture.m_pTex2D = Game2DBase::Create( readFile );
	}
	
	// �Ǘ�������悤�ɒm�点��
	CollisionManager::GetInstance()->AddUnit( this );
}

Collision2DUnit::~Collision2DUnit(void)
{
	m_drawTexture.DeleteAndInit();

	// �Ǘ����O���悤�ɒm�点��
	CollisionManager::GetInstance()->RemoveUnit( this );
}

//void Collision2DUnit::ListUpdate()
//{
//	uint32_t currBelongLv		= m_drawTexture.m_texInfo.m_belongLv;
//	uint32_t currBelongIndex	= m_drawTexture.m_texInfo.m_belongIndex;
//
//	if( currBelongLv != m_preBelongLv || currBelongIndex != m_preBelongIndex ){
//		// �V�����o�������X�g�ɓo�^��������
//		uint32_t index = CollisionManager::GetInstance()->GetRegisterTreeIndex( m_preBelongLv, m_preBelongIndex );
//		CollisionManager::GetInstance()->RemoveUnitFromTree( index, this );
//		index = CollisionManager::GetInstance()->GetRegisterTreeIndex( this );
//		CollisionManager::GetInstance()->RegisterUnitFromTree( index, this );
//		// �X�V
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
		// ���Ƃ��Ǝ��ɂ������j�b�g�ɏ��Z�b�g
		pCurrNext->SetPrevUnit( pUnit );

		// �V�������鎟�̃��j�b�g�ɏ��Z�b�g
		pUnit->SetNextUnit( pCurrNext );
		pUnit->SetPrevUnit( this );
	}
	// �����̎����X�V
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
		// ���Ƃ��ƌ��ɂ������j�b�g�ɏ��Z�b�g
		pCurrPrev->SetNextUnit( pUnit );

		// �V����������̃��j�b�g�ɏ��Z�b�g
		pUnit->SetNextUnit( this );
		pUnit->SetPrevUnit( pCurrPrev );
	}
	m_prevUnit = pUnit;
}
