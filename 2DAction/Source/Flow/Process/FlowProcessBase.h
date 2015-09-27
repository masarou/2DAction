/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		�e�t���[�ŕK�v�ȉ��o���L�q����x�[�X�N���X
 */
/* ====================================================================== */
#ifndef __FLOW_EFFECT_BASE__
#define __FLOW_EFFECT_BASE__

#include "System/Collision/SystemCollisionUnit.h"
#include "System/Input/SystemInputWatcher.h"
#include "Game/Game2DBase.h"

class ProcessBase : public InputWatcher
{

	friend FlowBase;

public:

	enum STATE{
		STATE_NONE,
		STATE_INIT,			// Init��
		STATE_UPDATE,		// Update��
		STATE_CANDIE,		// CanDie��
		STATE_FLOW_WAIT,	// �G�t�F�N�g�I���B�t���[�̑҂�

		STATE_MAX
	};

	// �G�t�F�N�g���I���������ǂ�������
	virtual bool IsEffectEnd(){ return (m_effectState == STATE_FLOW_WAIT) ? true : false ;}

protected:

	// �ȉ��A�h����Ŏ���
	virtual bool Init() = 0;
	virtual void Update() = 0;
	virtual bool CanDie(){ return true; }

	// ���o�Đ����ɍ����K�v���ǂ���
	virtual bool IsNeedPauseFilter() const{ return true; }

	// �N���X�̃X�e�[�^�X�擾�ƃZ�b�g
	const STATE &GetState() const{ return m_effectState; }
	void SetStateNext();

	ProcessBase(void);
	virtual ~ProcessBase(void);

private:

	// FlowBase����Ă΂ꖈ�t���[���X�V
	void Exec();

	STATE	m_effectState;
};

#endif