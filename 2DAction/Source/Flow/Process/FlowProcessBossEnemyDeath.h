/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		操作説明演出クラス
 */
/* ====================================================================== */
#ifndef __FLOW_PROCESS_BOSSENEMY_DEATH__
#define __FLOW_PROCESS_BOSSENEMY_DEATH__

#include "FlowProcessBase.h"

class ProcessBossEnemyDeath : public ProcessBase
{
public:

	static ProcessBossEnemyDeath *Create( const math::Vector2 &centerPos );
	
	ProcessBossEnemyDeath( const math::Vector2 &centerPos );
	~ProcessBossEnemyDeath(void);

protected:

	virtual bool Init() override;
	virtual void Update() override;
	virtual bool CanDie() override;
	virtual bool IsNeedPauseFilter() const override{ return false; }

	ProcessBossEnemyDeath(void);

	math::Vector2		m_centerPos;	// エフェクトの基準位置
};

#endif