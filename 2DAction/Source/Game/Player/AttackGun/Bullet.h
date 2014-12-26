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

#include "Game/Game2DBase.h"

// 固定値
static const uint32_t BULLET_LIVE_TIME	= 300;

class Bullet
{

public:
	Bullet( const uint32_t &uniqueNum, const math::Vector2 &pos, const math::Vector2 &vec, float speed );
	~Bullet(void);

	// 情報セット
	void	SetBulletVec( math::Vector2 &vec ){ m_bulletVec = vec; }
	void	SetBulletSpeed( float &spd ){ m_speed = spd; }

	void	Update();
	void	Draw();

	// 情報取得
	const uint32_t &GetBulletDamage() const{ return m_bulletDamage;}
	const TEX_DRAW_INFO &GetDrawInfo() const;
	const uint32_t &GetUniqueNumber() const{ return m_uniqueNumber; }
	const uint32_t &GetLiveTime() const{ return m_liveTime; }

private:
	
	uint32_t		m_uniqueNumber;	// ほかの弾と区別するためにユニーク番号
	uint32_t		m_liveTime;		// 生成されてからの時間
	Texture2D		m_textureBullet;// プレイヤー画像

	uint32_t		m_bulletDamage;		// 弾の威力
	math::Vector2	m_bulletVec;	// 発射方向
	float			m_speed;		// 発射スピード

};

#endif
