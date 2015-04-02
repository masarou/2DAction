/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		ステージ準備クラス
 */
/* ====================================================================== */
#ifndef __FLOW_PROCESS_INIT_LAST_STAGE__
#define __FLOW_PROCESS_INIT_LAST_STAGE__

#include "FlowProcessBase.h"

class InitLastStage : public ProcessBase
{
public:

	static InitLastStage *Create();

	~InitLastStage(void);

protected:

	virtual bool Init() override;
	virtual void Update() override;
	virtual bool CanDie() override;

	// パッド操作
	void PadEventDecide() override;

	InitLastStage(void);

	Texture2D			m_controllTex;	// 操作画像データ
};

#endif