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

class ProcessBase : public InputWatcher
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

	// エフェクトが終了したかどうか判定
	virtual bool IsEffectEnd(){ return (m_effectState == STATE_FLOW_WAIT) ? true : false ;}

protected:

	// 以下、派生先で実装
	virtual bool Init() = 0;
	virtual void Update() = 0;
	virtual bool CanDie(){ return true; }

	// 演出再生時に黒板が必要かどうか
	virtual bool IsNeedPauseFilter() const{ return true; }

	// クラスのステータス取得とセット
	const STATE &GetState() const{ return m_effectState; }
	void SetStateNext();

	ProcessBase(void);
	virtual ~ProcessBase(void);

private:

	// FlowBaseから呼ばれ毎フレーム更新
	void Exec();

	STATE	m_effectState;
};

#endif