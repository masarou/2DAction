/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		ボスクラス死亡時演出クラス
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

	uint32_t			m_effectCounter;// エフェクト再生時間
	math::Vector2		m_centerPos;	// エフェクトの基準位置

};

#endif