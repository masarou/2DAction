/* ====================================================================== */
/**
 * @brief  エフェクトクラス 基本一回再生+自殺
 *
 * @note
 *		
 */
/* ====================================================================== */

#include "Game/Game2DBase.h"
#include "System/SystemDefine.h"
#include "System/Task/SystemTaskUnit.h"
#include "System/Collision/SystemCollisionUnit.h"


#ifndef __GAME_EFFECT__
#define __GAME_EFFECT__

class GameEffect : public TaskUnit
{

public:

	enum EFFECT_KIND{
		EFFECT_BOMB,			// 敵がやられた時の爆発
		EFFECT_PRE_EXPLOSION,	// 爆発予兆
		EFFECT_DAMAGE,			// ダメージ
		EFFECT_EXCLAMATION,		// びっくりマーク		
		EFFECT_SLASHING_HIT,	// 斬撃HIT
		EFFECT_DASH_SMOKE,		// DASH煙
	};
	static GameEffect *CreateEffect( const EFFECT_KIND &kind, const math::Vector2 &pos );
	static GameEffect *CreateEffect( const EFFECT_KIND &kind, const int32_t &posX, const int32_t &posY );

protected:
	
	virtual bool Init() override;
	virtual bool DieMain() override;
	virtual void Update() override;
	virtual void DrawUpdate() override;

public:

	GameEffect( const EFFECT_KIND &kind, const math::Vector2 &pos );
	virtual ~GameEffect(void);

	std::string SelectEffectFile() const;		// 読み込みファイル選別関数

	EFFECT_KIND		m_kind;				// 演出の種類
	Texture2D		m_textureEffect;	// エフェクト

};

// 当たり判定付きゲームエフェクト
class GameEffectWithCollision : public TaskUnit, public Collision2DUnit
{

public:

	enum EFFECT_KIND{
		EFFECT_EXPLOSION,	// 爆発

		EFFECT_MAX
	};
	static GameEffectWithCollision *CreateEffect( const Common::OWNER_TYPE &owner, const EFFECT_KIND &kind, const math::Vector2 &pos );
	static GameEffectWithCollision *CreateEffect( const Common::OWNER_TYPE &owner, const EFFECT_KIND &kind, const int32_t &posX, const int32_t &posY );

protected:
	
	virtual bool Init() override;
	virtual void Update() override;
	virtual void DrawUpdate() override;
	
	// このクラスの種類セット
	virtual const Common::TYPE_OBJECT GetTypeObject() const override;

public:

	GameEffectWithCollision( const Common::OWNER_TYPE &owner, const EFFECT_KIND &kind, const math::Vector2 &pos );
	virtual ~GameEffectWithCollision(void);

	std::string SelectEffectFile() const;		// 読み込みファイル選別関数
	
	Common::OWNER_TYPE	m_ownerType;
	EFFECT_KIND			m_kind;				// 演出の種類

};

class GameEffectDamage : public TaskUnit
{

public:

	static GameEffectDamage *GetInstance();

	void CreateEffectDamage( const uint32_t &value, const int32_t &posX, const int32_t &posY );
	virtual ~GameEffectDamage(void);

protected:
	
	virtual bool Init() override;
	virtual bool DieMain() override;
	virtual void Update() override;
	virtual void DrawUpdate() override;

private:

	GameEffectDamage();

	struct EFFECT_DAMAGE_INFO{
		uint32_t m_value;
		uint32_t m_liveTime;
		std::vector<Texture2D>	m_array2D;

		void Init(){
			m_value		= 0;
			m_liveTime	= 0;
			m_array2D.clear();
		}
	};

	static GameEffectDamage	*s_pInstance;

	std::vector<EFFECT_DAMAGE_INFO> m_damageArray;
};

#endif