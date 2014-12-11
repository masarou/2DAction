/* ====================================================================== */
/**
 * @brief  プレイヤーの攻撃管理クラス
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

// 固定値
static const uint32_t SHOOT_INTERBAL_DEFAULT	= 16;

class AttackGun : public TaskUnit
{

public:

	struct GunState{
		uint32_t	m_shootInterval;	// 発射間隔
		float		m_speed;			// 発射スピード
		uint32_t	m_damage;			// ダメージ

		void Init(){
			m_shootInterval	= SHOOT_INTERBAL_DEFAULT;
			m_speed			= 10.0f;
			m_damage		= 1;
		}
	};

	AttackGun(void);
	~AttackGun(void);
	
	virtual void Update() override;
	virtual void DrawUpdate() override;
	virtual bool DieMain() override;

	// 弾の発射
	void ShootBullet( math::Vector2 pos = math::Vector2(), math::Vector2 vec = math::Vector2( 0.0f, 1.0f ) );
	// 弾の削除(画面外に出た、敵に当たった等々)
	std::vector<Bullet*>::iterator DeleteBullet( uint32_t uniqueNumber );
	// 銃のステート更新
	GunState &UpdateGunState(){ return m_currState; }

private:

	GunState				m_currState;	// 銃のステータス
	uint32_t				m_intervalTime; // 発射の間隔
	std::vector<Bullet*>	m_magazine;		// 弾の管理ベクタ

};

#endif

