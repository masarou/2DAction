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

class Slashing;

class AttackBlade : public TaskUnit
{

public:

	struct BladeState{

		uint32_t	m_damage;			// ダメージ

		void Init(){
			m_damage	= 20;
		}
	};

	static AttackBlade *CreateAttackBlade( const Common::OWNER_TYPE &ownerType );
	~AttackBlade(void);

	// 斬撃生成
	void CreateSlashing(  const math::Vector2 &pos, const math::Vector2 &vec );

	// 剣のステータス
	BladeState &UpdateBladeState(){ return m_currState; }

	// 現在攻撃中(斬撃アニメが出ている)かどうか
	bool IsSlashingAnimPlay(){ return m_currSlashing ? true : false ;}

protected:
	
	virtual void Update() override;
	virtual bool DieMain() override;
	virtual void MessageReceive( const Message &msg );

private:

	struct RESERVE_SLASHING_INFO{
		bool			m_isReserve;
		math::Vector2	m_pos;
		math::Vector2	m_vec;
		void Init(){
			m_isReserve = false;
			m_pos = math::Vector2();
			m_vec = math::Vector2();
		}
	};

	AttackBlade( const Common::OWNER_TYPE &ownerType );
	
	Common::OWNER_TYPE		m_owner;
	BladeState				m_currState;		// 剣のステータス
	uint32_t				m_intervalTime;		// 斬撃生成の間隔
	RESERVE_SLASHING_INFO	m_reserveInfo;		// 斬撃予約情報

	Slashing				*m_currSlashing;	// 生成中の斬撃
};

#endif
