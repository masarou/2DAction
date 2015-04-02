/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		操作説明演出クラス
 */
/* ====================================================================== */
#ifndef __FLOW_PROCESS_CTRL_DESCRIPTION__
#define __FLOW_PROCESS_CTRL_DESCRIPTION__

#include "FlowProcessBase.h"

class ControllDescription : public ProcessBase
{
public:

	static ControllDescription *Create();

	~ControllDescription(void);

protected:

	virtual bool Init() override;
	virtual void Update() override;
	virtual bool CanDie() override;

	// パッド操作
	void PadEventDecide() override;

	ControllDescription(void);

	Texture2D			m_controllTex;	// 操作画像データ
};

#endif