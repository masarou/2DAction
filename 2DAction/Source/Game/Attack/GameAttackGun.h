/* ====================================================================== */
/**
 * @brief  プレイヤーの攻撃管理クラス
 *
 * @note
 *		Bladeクラスをベクタで持つ
 *		実際に的とのやり取りはBladeクラスが行いこのクラスはBladeクラスと他クラスとの仲介
 *		CmnGameAccesserを通じてこのクラスが管理しているBladeクラスの位置情報を取得したりする
 *		
 */
/* ====================================================================== */

#ifndef __SYSTEM_ATTACK_GUN__
#define __SYSTEM_ATTACK_GUN__

#include "Common/CommonDefine.h"
#include "System/Task/SystemTaskUnit.h"


// 固定値
static const uint32_t SHOOT_TO_OVERHEAT_DEFAULT	= 12;
static const uint32_t SHOOT_SPEED_DEFAULT		= 20;
static const uint32_t SHOOT_DAMAGE_DEFAULT		= 25;
static const uint32_t SHOOT_INTERVAL_DEFAULT	= 20;

class Bullet;

class AttackGun : public TaskUnit
{

public:

	struct GunState{
		uint32_t	m_damageLv;			// マシンガンレベル
		uint32_t	m_intervalLv;		// マシンガンレベル
		uint32_t	m_timeToOverHeat;	// オーバーヒートまでの時間
		uint32_t	m_shootInterval;	// 発射間隔
		uint32_t	m_speed;			// 発射スピード
		uint32_t	m_damage;			// ダメージ

		void Init(){
			m_damageLv		= 0;
			m_intervalLv	= 0;
			m_timeToOverHeat= SHOOT_TO_OVERHEAT_DEFAULT;
			m_shootInterval	= SHOOT_INTERVAL_DEFAULT;
			m_speed			= SHOOT_SPEED_DEFAULT;
			m_damage		= SHOOT_DAMAGE_DEFAULT;
		}
	};

	static AttackGun *CreateGun( const Common::OWNER_TYPE &ownerType ); 
	~AttackGun(void);

	// 弾の発射(登録)と削除
	void ShootBullet( const math::Vector2 &pos, const math::Vector2 &vec );
	void ShootBullet( const math::Vector2 &pos, const math::Vector2 &vec, const uint32_t &damage, const uint32_t &speed );

	// 銃のステート更新
	GunState &UpdateGunState(){ return m_currState; }

	// マシンガンのLv設定とステータス反映
	void SetGunLevel( const uint32_t &damageLv, const uint32_t &speedLv );

protected:
	
	virtual void Update() override;
	virtual bool DieMain() override;

private:
	
	AttackGun( const Common::OWNER_TYPE &ownerType );

	Common::OWNER_TYPE		m_owner;
	bool					m_isOverHeat;
	uint32_t				m_overHeatTime;
	GunState				m_currState;	// 銃のステータス
	uint32_t				m_intervalTime; // 発射の間隔(一発単位)
	uint32_t				m_totalIntervalTime; // 発射の間隔(トータル)
	std::vector<Bullet*>	m_magazine;		// 弾の管理ベクタ

};

#endif

