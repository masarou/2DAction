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

#ifndef __SYSTEM_ATTACK_GUN__
#define __SYSTEM_ATTACK_GUN__

#include "Common/CommonDefine.h"
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
			m_speed			= 15.0f;
			m_damage		= 20;
		}
	};

	static AttackGun *CreateGun( const Common::OWNER_TYPE &ownerType ); 
	~AttackGun(void);

	// 弾の発射(登録)と削除
	void ShootBullet( math::Vector2 pos = DEFAULT_VECTOR2, math::Vector2 vec = math::Vector2( 0.0f, 1.0f ) );

	// 銃のステート更新
	GunState &UpdateGunState(){ return m_currState; }

protected:

	virtual void Update() override;
	virtual bool DieMain() override;

private:
	
	AttackGun( const Common::OWNER_TYPE &ownerType );

	Common::OWNER_TYPE		m_owner;
	GunState				m_currState;	// 銃のステータス
	uint32_t				m_intervalTime; // 発射の間隔
	std::vector<Bullet*>	m_magazine;		// 弾の管理ベクタ

};

#endif

