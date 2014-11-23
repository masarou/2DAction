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

	};

	GameEffect( const EFFECT_KIND &kind, const uint32_t &posX, const uint32_t &posY );
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

#endif