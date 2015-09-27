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

	enum TYPE_SLASHING{
		TYPE_1ST,
		TYPE_2ND,
		TYPE_3RD,

		TYPE_MAX,
	};

	static const uint32_t SLASHING_ANIM_END = 0;

	Slashing( const Common::OWNER_TYPE ownerType, const TYPE_SLASHING &type, const math::Vector2 &pos, const math::Vector2 &vec, uint32_t damage, bool deleteBullet = false );
	~Slashing(void);

	// 情報取得
	const TYPE_SLASHING &GetTypeSlashing() const{ return m_slashingType; }
	const uint32_t &GetBladeDamage() const{ return m_bladeDamage; }
	const TEX_DRAW_INFO &GetDrawInfo() const;
	const uint32_t &GetLiveTime() const{ return m_liveTime; }
	const bool &IsDeleteBullet() const{ return m_IsDeleteBullet; }

protected:

	virtual bool Init() override;
	virtual bool DieMain() override;
	virtual void Update() override;			// 移動等の内部数値の更新
	virtual void DrawUpdate() override;		// 描画更新

	// ほかのクラスからのイベント処理
	virtual void EventUpdate( Common::CMN_EVENT &eventId ) override;

	// このクラスの種類セット
	virtual const Common::TYPE_OBJECT GetTypeObject() const override;

private:

	// 当たり判定が有効となるフレーム取得
	void GetHitFrame();

	const std::string GetJsonFileStr();
	
	Common::OWNER_TYPE		m_ownerType;
	TYPE_SLASHING			m_slashingType;
	uint32_t				m_liveTime;			// 生成されてからの時間

	bool					m_IsDeleteBullet;	// 敵の弾をかき消せるか
	uint32_t				m_bladeDamage;		// 斬撃威力
	math::Vector2			m_bladePos;			// 斬撃位置
	math::Vector2			m_bladeVec;			// 斬撃向き
	std::vector<uint32_t>	m_hitFrame;			// 当たり判定が有効となるフレーム

};

#endif
