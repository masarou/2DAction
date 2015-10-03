/* ====================================================================== */
/**
 * @brief  敵AIの攻撃クラス(遠距離)
 *
 * @note
 *		攻撃AI
 */
/* ====================================================================== */

#ifndef __ENEMY_AI_SHOOT__
#define __ENEMY_AI_SHOOT__

#include "EnemyAIBase.h"

class EnemyAIShoot : public EnemyAIBase
{
public:

	static EnemyAIShoot *Create();

	// AIの種類を派生先でセットしておく
	virtual const Common::ENEMY_AI GetAIKind() const{ return Common::AI_SHOOTING; }

protected:
	
	virtual bool InitAI() override{ return true; }					// AI初期化
	virtual void ExecMain( TEX_DRAW_INFO &enemyInfo, ACTION_ARRAY &actionInfo ) override;		// 派生先でのAI実装

private:

	EnemyAIShoot(void);
	~EnemyAIShoot(void);

	uint32_t	m_shootInterval;	// 連続して発射するまでの最低時間
};
#endif
