/* ====================================================================== */
/**
 * @brief  敵AIの攻撃クラス(ダッシュで突撃)
 *
 * @note
 *		攻撃AI
 */
/* ====================================================================== */

#ifndef __ENEMY_AI_DASH_TACKLE__
#define __ENEMY_AI_DASH_TACKLE__

#include "EnemyAIBase.h"

class EnemyAIDashTackle : public EnemyAIBase
{

public:

	static EnemyAIDashTackle *Create();

	// AIの種類を派生先でセットしておく
	virtual const Common::ENEMY_AI GetAIKind() const{ return Common::AI_DASH_TACKLE; }

protected:
	
	virtual bool InitAI() override;								// AI初期化
	virtual void ExecMain( TEX_DRAW_INFO &enemyInfo, ACTION_ARRAY &actionInfo ) override;	// 派生先でのAI実装

	void UpdateIdle( TEX_DRAW_INFO &enemyInfo, ACTION_ARRAY &actionInfo );
	void UpdateAction( TEX_DRAW_INFO &enemyInfo, ACTION_ARRAY &actionInfo );
	void UpdatePreEnd( TEX_DRAW_INFO &enemyInfo, ACTION_ARRAY &actionInfo );

private:

	enum TACKLE_STATE{
		IDLE,
		ACTION,
		PRE_END,

		MAX,
	};

	EnemyAIDashTackle(void);
	~EnemyAIDashTackle(void);

	TACKLE_STATE	m_tackleState;
	uint32_t		m_actionTime;		// ACTIONにステータスが移ってからの時間
	math::Vector2	m_tackleMoveVec;	// タックル方向
	float			m_actionAccelSpeed;	// タックルスピード
	float			m_tacklePower;		// タックル力

};
#endif