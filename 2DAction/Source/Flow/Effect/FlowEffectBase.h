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

class FlowEffectBase : public InputWatcher
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

	void Exec();
	virtual bool IsEffectEnd(){ return (m_effectState == STATE_FLOW_WAIT) ? true : false ;}
	virtual ~FlowEffectBase(void);

protected:

	virtual bool Init() = 0;
	virtual void Update() = 0;
	virtual bool CanDie(){ return true; }

	const STATE &GetState() const{ return m_effectState; }
	void SetStateNext();

	FlowEffectBase(void);

private:

	STATE	m_effectState;
};

#endif