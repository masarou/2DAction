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
{
	// �`��N���X�Z�b�g�A�b�v
	m_drawTexture.Init();
	if( readFile ){
		m_drawTexture.m_pTex2D = NEW Game2DBase( readFile );
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

const TEX_DRAW_INFO &Collision2DUnit::GetDrawInfo() const
{
	return m_drawTexture.m_texInfo;
}
