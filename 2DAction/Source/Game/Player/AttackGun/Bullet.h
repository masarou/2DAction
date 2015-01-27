/* ====================================================================== */
/**
 * @brief  GamePlayerAttackGunから発射される球クラス
 *
 * @note
 *		GamePlayerAttackGunを親として発射された球のクラス
 *		一発の玉の情報を持ち、スピードや方向、位置情報を持つ
 *		
 */
/* ====================================================================== */

#ifndef ATTACK_BULLET
#define ATTACK_BULLET

#include "Common/CommonDefine.h"
#include "Game/Game2DBase.h"
#include "System/Task/SystemTaskUnit.h"
#include "System/Collision/SystemCollisionUnit.h"

// 固定値
static const uint32_t BULLET_LIVE_TIME	= 180;

class Bullet : public TaskUnit, public Collision2DUnit
{

public:

	Bullet( const Common::OWNER_TYPE ownerType, const uint32_t &uniqueNum, const math::Vector2 &pos, const math::Vector2 &vec, float speed );
	~Bullet(void);

	// 情報セット
	void	SetBulletVec( math::Vector2 &vec ){ m_bulletVec = vec; }
	void	SetBulletSpeed( float &spd ){ m_speed = spd; }

	// 情報取得
	const uint32_t &GetBulletDamage() const{ return m_bulletDamage;}
	const TEX_DRAW_INFO &GetDrawInfo() const;
	const uint32_t &GetUniqueNumber() const{ return m_uniqueNumber; }
	const uint32_t &GetLiveTime() const{ return m_liveTime; }

protected:

	virtual bool Init() override;
	virtual bool DieMain() override;
	virtual void Update() override;			// 移動等の内部数値の更新
	virtual void CollisionUpdate() override;// 内部数値の更新を受けての他クラスとの当たり判定処理
	virtual void DrawUpdate() override;		// 描画更新

	// ほかのクラスからのイベント処理
	virtual void EventUpdate( const Common::CMN_EVENT &eventId ) override;

	// このクラスの種類セット
	virtual const Common::TYPE_OBJECT GetTypeObject() const override;

private:
	
	Common::OWNER_TYPE	m_ownerType;
	uint32_t			m_uniqueNumber;	// ほかの弾と区別するためにユニーク番号
	uint32_t			m_liveTime;		// 生成されてからの時間

	uint32_t			m_bulletDamage;	// 弾の威力
	math::Vector2		m_bulletVec;	// 発射方向
	float				m_speed;		// 発射スピード

};

#endif
