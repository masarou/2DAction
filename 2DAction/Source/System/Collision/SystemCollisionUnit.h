/* ====================================================================== */
/**
 * @brief  SystemCollisionUnit.h
 *
 * @note	�����蔻����s��2D�N���X�͂�������Ƃ��ĉ摜��\������
 *			SystemMessageUnit���p�����Ă���A�����蔻���EventUpdate�Ŏ擾
 */
/* ====================================================================== */

#ifndef __SYSTEM_COLLISIONUNIT__
#define __SYSTEM_COLLISIONUNIT__

#include "System/Message/SystemMessageUnit.h"
#include "Game/Game2DBase.h"

class Collision2DUnit : public SystemMessageUnit
{
public:

	// ���������҂Ȃ̂��������^�C�v
	virtual const Common::TYPE_OBJECT GetTypeObject() const = 0 ;

	// �`����擾
	const TEX_DRAW_INFO &GetDrawInfo() const;

protected:
	
	Collision2DUnit( const char *readFile = NULL );
	virtual ~Collision2DUnit(void);
	Texture2D			m_drawTexture;	// �\���摜�f�[�^

private:


};

#endif