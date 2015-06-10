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

#include "Game/Game2DBase.h"
#include "System/Message/SystemMessageUnit.h"

class Collision2DUnit : public SystemMessageUnit
{
public:

	// ���������҂Ȃ̂��������^�C�v
	virtual const Common::TYPE_OBJECT GetTypeObject() const = 0 ;

	// CollisionManager����Ă΂��X�V�֐�
	//void ListUpdate();

	// �����蔻������邩�ǂ����t���O�Z�b�g
	void SetInvalidCollisionFlag( const bool &flg ){ m_invalidCollision = flg; }
	const bool GetInvalidCollisionFlag() const{ return m_invalidCollision; }

	// �`����擾
	const TEX_DRAW_INFO &GetDrawInfo() const;

	// �����蔻�����̏��擾
	const uint32_t &GetBelongLv() const;
	const uint32_t &GetBelongIndex() const;

	// �o�������X�g�Ɏw��N���X��o�^
	void ClearChainList(){ m_nextUnit = NULL; m_prevUnit = NULL; }
	Collision2DUnit *GetNextUnit(){ return m_nextUnit; }
	Collision2DUnit *GetPrevUnit(){ return m_prevUnit; }
	void SetNextUnit( Collision2DUnit *pUnit );
	void SetPrevUnit( Collision2DUnit *pUnit );

protected:
	
	Collision2DUnit( const char *readFile = NULL );
	virtual ~Collision2DUnit(void);
	Texture2D			m_drawTexture;	// �\���摜�f�[�^

private:

	// ��������邩�ǂ����t���O
	bool				m_invalidCollision;

	// �����蔻��Ŏg�p����o�������X�g
	Collision2DUnit		*m_nextUnit;
	Collision2DUnit		*m_prevUnit;

	// ���O�̋�ԏ��
	uint32_t			m_preBelongLv;
	uint32_t			m_preBelongIndex;
};

#endif