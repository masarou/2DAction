/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		各フローで必要な演出を記述するベースクラス
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
		STATE_INIT,			// Init中
		STATE_UPDATE,		// Update中
		STATE_CANDIE,		// CanDie中
		STATE_FLOW_WAIT,	// エフェクト終了。フローの待ち

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