/* ====================================================================== */
/**
 * @brief  斬撃アニメ
 *
 * @note
 *		一回再生→自殺 他のUnitとの当たり判定があるので相応のものを継承
 *		
 */
/* ====================================================================== */

#ifndef __BLADE__
#define __BLADE__

#include "Common/CommonDefine.h"
#include "Game/Game2DBase.h"
#include "System/Task/SystemTaskUnit.h"
#include "System/Collision/SystemCollisionUnit.h"

class Slashing : public TaskUnit, public Collision2DUnit
{

public:

	static const uint32_t SLASHING_ANIM_END = 0;

	Slashing( const Common::OWNER_TYPE ownerType, const math::Vector2 &pos, const math::Vector2 &vec, uint32_t damage );
	~Slashing(void);

	// 情報取得
	const uint32_t &GetBladeDamage() const{ return m_bladeDamage; }
	const TEX_DRAW_INFO &GetDrawInfo() const;
	const uint32_t &GetLiveTime() const{ return m_liveTime; }

protected:

	virtual bool Init() override;
	virtual bool DieMain() override;
	virtual void Update() override;			// 移動等の内部数値の更新
	virtual void DrawUpdate() override;		// 描画更新

	// ほかのクラスからのイベント処理
	virtual void EventUpdate( const Common::CMN_EVENT &eventId ) override;

	// このクラスの種類セット
	virtual const Common::TYPE_OBJECT GetTypeObject() const override;

private:
	
	Common::OWNER_TYPE	m_ownerType;
	uint32_t			m_liveTime;		// 生成されてからの時間

	uint32_t			m_bladeDamage;	// 斬撃威力
	math::Vector2		m_bladePos;		// 斬撃位置
	math::Vector2		m_bladeVec;		// 斬撃向き

};

#endif
