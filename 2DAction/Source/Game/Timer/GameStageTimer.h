/* ====================================================================== */
/**
 * @brief  制限時間ステージの残り時間を表すクラス
 *
 * @note
 *		
 */
/* ====================================================================== */

#ifndef __GAME_STAGE_TIMER__
#define __GAME_STAGE_TIMER__

#include "Common/CommonDefine.h"
#include "Game/Game2DBase.h"
#include "System/Task/SystemTaskUnit.h"

class GameCountDownTimer;

class StageTimer : public TaskUnit
{

public:

	static StageTimer *CreateStageTimer( uint32_t startTime );
	~StageTimer(void);

protected:

	virtual bool Init() override;
	virtual bool DieMain() override;
	virtual void Update() override;			// 移動等の内部数値の更新
	virtual void DrawUpdate() override;		// 描画更新

private:
	
	StageTimer( uint32_t startTime );

	// 指定パーツの情報取得
	const math::Vector2 GetPartsPos( const std::string name ) const;
	const Common::PARTS_INFO &GetPartsInfo( const std::string name ) const;

	uint32_t			m_startTime;		// カウント開始数

	// 描画
	Texture2D			m_plateStageTimer;	// ステージタイマーの下地
	GameCountDownTimer	*m_pCountDown;		// 実際にカウントダウンする数字クラス
	Texture2D			m_animClock;		// 砂時計

	// パーツマップ
	std::map< std::string, Common::PARTS_INFO >	m_partsMap;
};

#endif
