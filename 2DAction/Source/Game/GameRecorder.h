/* ====================================================================== */
/**
 * @brief	フローをまたいだデータ置き場
 *			スコアやプレイヤーのステータス情報等
 *
 * @note
 *		
 */
/* ====================================================================== */

#ifndef __SCORE_RECORDER__
#define __SCORE_RECORDER__

#include "System/SystemDefine.h"
#include "System/Task/SystemTaskUnit.h"

class GameRecorder : public TaskUnit
{

public:

	// 得点が入る種類
	enum SCORE_KIND {
		ENEMY_AAA_DEATH,
		ENEMY_BBB_DEATH,
		ENEMY_CCC_DEATH,

		POINT_MAX
	};

	// ゲームの進行度
	enum STATE_OF_PROGRESS{
		STATE_TITLE,
		STATE_STAGE01,
		STATE_STAGE02,
		STATE_STAGE03,

		STATE_MAX,
	};

	static GameRecorder *Create();
	static GameRecorder *GetInstance();
	~GameRecorder();

	// 初期化
	void InitRecord();

	// スコアの加算と取得
	void ScoreEvent( const SCORE_KIND &kind );
	const int32_t GetScore() const;

	// ゲーム進行状況の更新と取得
	void SetGameStateOfProgress( STATE_OF_PROGRESS nextState );
	const STATE_OF_PROGRESS &GetGameStateOfProgress() const;

private:

	GameRecorder();

	static GameRecorder *s_pInstance;

	STATE_OF_PROGRESS	m_gameState;				// ゲーム進行度

	uint32_t			m_userScore;				// ユーザースコア
	uint32_t			m_scoreDetail[POINT_MAX];	// スコア詳細
};
#endif