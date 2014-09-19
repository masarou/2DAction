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

	GameEffect( EFFECT_KIND kind, uint32_t posX, uint32_t posY );
	~GameEffect(void);

protected:

	virtual bool DieMain() override;
	virtual void Update() override;
	virtual void DrawUpdate() override;

public:

	std::string SelectEffectFile();		// 読み込みファイル選別関数
	std::string SelectEffectAnimTag();	// 再生アニメタグ取得

	EFFECT_KIND			m_kind;			// 演出の種類
	Game2DBase			*m_pEffect;		// 描画クラス本体
	TEX_DRAW_INFO		m_effectInfo;	// 描画情報
	
};

#endif