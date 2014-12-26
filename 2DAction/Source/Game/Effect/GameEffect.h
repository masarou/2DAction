/* ====================================================================== */
/**
 * @brief  エフェクトクラス 基本一回再生+自殺
 *
 * @note
 *		
 */
/* ====================================================================== */

#include "System/SystemDefine.h"
#include "System/Task/SystemTaskUnit.h"
#include "Game/Game2DBase.h"


#ifndef GAME_EFFECT
#define GAME_EFFECT

class GameEffect : public TaskUnit
{

public:

	enum EFFECT_KIND{
		EFFECT_BOMB,		// 爆発
		EFFECT_DAMAGE,		// ダメージ
	};

	GameEffect( const EFFECT_KIND &kind, const int32_t &posX, const int32_t &posY );
	virtual ~GameEffect(void);

protected:
	
	virtual bool Init() override;
	virtual bool DieMain() override;
	virtual void Update() override;
	virtual void DrawUpdate() override;

public:

	std::string SelectEffectFile();		// 読み込みファイル選別関数
	std::string SelectEffectAnimTag();	// 再生アニメタグ取得

	EFFECT_KIND		m_kind;				// 演出の種類
	Texture2D		m_textureEffect;	// エフェクト

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