/* ====================================================================== */
/**
 * @brief  プレイヤークラス
 *
 * @note
 *		Bulletクラスをベクタで持つ
 *		実際に的とのやり取りはBulletクラスが行いこのクラスはBulletクラスと他クラスとの仲介
 *		CmnGameAccesserを通じてこのクラスが管理しているBulletクラスの位置情報を取得したりする
 *		
 */
/* ====================================================================== */

#ifndef SYSTEM_ATTACK_GUN
#define SYSTEM_ATTACK_GUN

#include "System/Task/SystemTaskUnit.h"
#include "Bullet.h"

class AttackGun : public TaskUnit
{

public:

	AttackGun(void);
	~AttackGun(void);
	
	virtual void Update() override;
	virtual void DrawUpdate() override;
	virtual bool DieMain() override;

	// 弾の発射
	void ShootBullet( math::Vector2 pos = math::Vector2(), math::Vector2 vec = math::Vector2( 0.0f, 1.0f ) );
	// 弾の削除(画面外に出た、敵に当たった等々)
	void DeleteBullet( uint32_t uniqueNumber );

private:

	uint32_t				m_intervalTime; // 発射の間隔
	std::vector<Bullet*>	m_magazine;		// 弾の管理ベクタ

};

#endif

