/* ====================================================================== */
/**
 * @brief  スコア記録クラス
 *
 * @note
 *		
 */
/* ====================================================================== */

#ifndef SCORE_RECORDER
#define SCORE_RECORDER

#include "Game/Game2DBase.h"
#include "System/SystemDefine.h"
#include "System/Task/SystemTaskUnit.h"
#include "Common/CmnNumberCounter.h"

class ScoreRecorder : public TaskUnit
{

public:

	// 得点が入る種類
	enum SCORE_KIND {
		ENEMY_AAA_DEATH,
		ENEMY_BBB_DEATH,
		ENEMY_CCC_DEATH,

		POINT_MAX
	};

	static ScoreRecorder *Create();
	static ScoreRecorder *GetInstance();
	~ScoreRecorder();

	// 初期化
	void ScoreReset();
	// スコア追加
	void ScoreEvent( const SCORE_KIND &kind );
	// スコア取得
	const int32_t GetScore() const;

private:

	ScoreRecorder();

	static ScoreRecorder *s_pInstance;
	uint32_t	m_scoreDetail[POINT_MAX];
	NumberCounter	*m_pNumCounter;
};
#endif