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

	// 親へ投げるメッセージ定義
	static const uint32_t EFFECT_ANIM_END = 0;

	enum EFFECT_KIND{
		EFFECT_BOMB,			// 敵がやられた時の爆発
		EFFECT_PRE_EXPLOSION,	// 爆発予兆
		EFFECT_DAMAGE,			// ダメージ
		EFFECT_EXCLAMATION,		// びっくりマーク
		EFFECT_SLASHING_HIT,	// 斬撃HIT
		EFFECT_DASH_SMOKE,		// DASH煙
		EFFECT_INVALID_DAMAGE,	// ダメージ無効
		EFFECT_WORP,			// ワープ
		EFFECT_PRE_FIRE_WALL,	// 画面全体炎演出
		EFFECT_FIRE_WALL,		// 画面全体炎演出
		EFFECT_SUMMON,			// 召喚
		EFFECT_FIRE_FLOWER,		// クリア後花火
		EFFECT_HEALING,			// ヒールエフェクト
	};

	static GameEffect *CreateEffect( const EFFECT_KIND &kind, const int32_t &posX, const int32_t &posY );
	static GameEffect *CreateEffect( const EFFECT_KIND &kind, const math::Vector2 &pos );

	void SetEffectAnim( const std::string &animStr );
	void SetDrawFlag( const bool &isDraw ){ m_isInvalid = !isDraw; }

protected:
	
	virtual bool Init() override;
	virtual bool DieMain() override;
	virtual void Update() override;
	virtual void DrawUpdate() override;

public:

	GameEffect( const EFFECT_KIND &kind, const math::Vector2 &pos );
	virtual ~GameEffect(void);

	std::string SelectEffectFile() const;		// 読み込みファイル選別関数

	bool			m_isInvalid;		// 描画無効フラグ
	EFFECT_KIND		m_kind;				// 演出の種類
	Texture2D		m_textureEffect;	// エフェクト

};

class PreFireWall : public GameEffect
{
public:

	PreFireWall( const EFFECT_KIND &kind, const math::Vector2 &pos );

protected:

	virtual void Update() override;

private:

	virtual ~PreFireWall(void);

	uint32_t	m_liveTime;	// 生成してからの時間
};

// 当たり判定付きゲームエフェクト
class GameEffectWithCollision : public TaskUnit, public Collision2DUnit
{

public:

	enum EFFECT_KIND{
		EFFECT_EXPLOSION,	// 爆発
		EFFECT_FIRE,		// 炎
		EFFECT_POISON,		// 毒

		EFFECT_MAX
	};
	static GameEffectWithCollision *CreateEffect( const Common::OWNER_TYPE &owner, const EFFECT_KIND &kind, const math::Vector2 &pos );
	static GameEffectWithCollision *CreateEffect( const Common::OWNER_TYPE &owner, const EFFECT_KIND &kind, const int32_t &posX, const int32_t &posY );

protected:

	GameEffectWithCollision( const Common::OWNER_TYPE &owner, const EFFECT_KIND &kind, const math::Vector2 &pos );
	virtual ~GameEffectWithCollision(void);
	
	virtual bool Init() override;
	virtual void DrawUpdate() override;
	
	// このクラスの種類セット
	virtual const Common::TYPE_OBJECT GetTypeObject() const override;

private:

	std::string SelectEffectFile() const;		// 読み込みファイル選別関数
	
	Common::OWNER_TYPE	m_ownerType;
	EFFECT_KIND			m_kind;				// 演出の種類

};

class FireWithCollision : public GameEffectWithCollision
{

public:

	FireWithCollision( const Common::OWNER_TYPE &owner, const EFFECT_KIND &kind, const math::Vector2 &pos );

protected:

	virtual void Update() override;

private:

	virtual ~FireWithCollision(void);

	uint32_t	m_liveTime;	// 生成してからの時間
};

class GameEffectDamage : public TaskUnit
{

public:

	static GameEffectDamage *GetInstance();

	void CreateEffectDamage( const uint32_t &value, const int32_t &posX, const int32_t &posY, bool isPlayer = false );
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

class GameEffectLoop : public TaskUnit
{

public:

	enum EFFECT_KIND{
		EFFECT_STATUS_ICON		// 異常状態を表すアイコン
	};

	static GameEffectLoop *CreateEffect( const EFFECT_KIND &kind, const int32_t &posX, const int32_t &posY );
	static GameEffectLoop *CreateEffect( const EFFECT_KIND &kind, const math::Vector2 &pos );

	void SetEffectAnim( const std::string &animStr );
	void SetDrawFlag( const bool &isDraw ){ m_isInvalid = !isDraw; }

protected:
	
	virtual bool Init() override;
	virtual bool DieMain() override;
	virtual void Update() override;
	virtual void DrawUpdate() override;

public:

	GameEffectLoop( const EFFECT_KIND &kind, const math::Vector2 &pos );
	virtual ~GameEffectLoop(void);

	std::string SelectEffectFile() const;		// 読み込みファイル選別関数

	bool			m_isInvalid;		// 描画無効フラグ
	EFFECT_KIND		m_kind;				// 演出の種類
	Texture2D		m_textureEffect;	// エフェクト

};

#endif