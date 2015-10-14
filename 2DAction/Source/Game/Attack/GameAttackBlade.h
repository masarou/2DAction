///* ====================================================================== */
///**
// * @brief  斬撃アニメ管理クラス
// *
// * @note
// *		斬撃を連続で出すときの時間管理、威力や種類を担う
// *		
// */
///* ====================================================================== */

#ifndef __ATTACK_BLADE__
#define __ATTACK_BLADE__

#include "Common/CommonDefine.h"
#include "Game/Game2DBase.h"
#include "System/Task/SystemTaskUnit.h"
#include "Slashing.h"

// 固定値
static uint32_t SLASHING_INTERBAL_DEFAULT	= 20;	// 斬撃の生成間隔
static uint32_t SLASHING_DAMAGE_DEFAULT		= 35;	// 斬撃の威力

class AttackBlade : public TaskUnit
{

public:

	struct BladeState{

		uint32_t	m_bladeLv;			// 斬撃Lv
		uint32_t	m_damage;			// ダメージ
		uint32_t	m_interval;			// 斬撃間隔
		bool		m_deleteBullet;		// 弾をかき消せるか

		void Init(){
			m_bladeLv		= 0;
			m_damage		= SLASHING_DAMAGE_DEFAULT;
			m_interval		= 0;
			m_deleteBullet	= false;
		}
	};

	static AttackBlade *CreateAttackBlade( const Common::OWNER_TYPE &ownerType );
	~AttackBlade(void);

	// 斬撃生成
	void CreateSlashing( const math::Vector2 &pos, const math::Vector2 &vec, const Slashing::TYPE_SLASHING &type = Slashing::TYPE_1ST, const uint32_t &damage = 0 );

	// 剣のステータス
	BladeState &UpdateBladeState(){ return m_currState; }

	// 剣のLvの設定とステータス反映
	void SetBladeLevel( const uint32_t &level );

	// 現在攻撃中(斬撃アニメが出ている)かどうか
	bool IsSlashingAnimPlay(){ return m_currSlashing ? true : false ;}

protected:
	
	virtual void Update() override;
	virtual bool DieMain() override;
	virtual void MessageReceive( const Message &msg );

private:

	struct RESERVE_SLASHING_INFO{
		bool					m_isReserve;
		Slashing::TYPE_SLASHING	m_type;
		math::Vector2			m_pos;
		math::Vector2			m_vec;
		void Init(){
			m_isReserve	= false;
			m_type		= Slashing::TYPE_MAX;
			m_pos		= math::Vector2();
			m_vec		= math::Vector2();
		}
	};
	
	// 斬撃生成
	void CreateSlashingMain( const math::Vector2 &pos, const math::Vector2 &vec, const Slashing::TYPE_SLASHING &type, const uint32_t &damage );

	AttackBlade( const Common::OWNER_TYPE &ownerType );
	
	Common::OWNER_TYPE		m_owner;
	BladeState				m_currState;		// 剣のステータス
	uint32_t				m_intervalTime;		// 斬撃生成の間隔
	RESERVE_SLASHING_INFO	m_reserveInfo;		// 斬撃予約情報

	Slashing				*m_currSlashing;	// 生成中の斬撃
};

#endif
