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
#include "Item/ItemObject.h"

class GameRecorder : public TaskUnit
{

public:

	static const uint32_t COMBO_COUNT_MAX = 10000;

	// 得点が入る種類
	enum SCORE_KIND {
		ENEMY_SLIME_DEATH,
		ENEMY_SLIME_ANOTHER_DEATH,
		ENEMY_AHRIMAN_DEATH,
		ENEMY_COW_DEATH,
		ENEMY_BOSS_DEATH,
		ENEMY_SLIME_KING_DEATH,
		ENEMY_WIZARD_DEATH,
		ENEMY_DRAGON_DEATH,
		ENEMY_LAST_BOSS_DEATH,
		ENEMY_LAST_BOSS_RIGHT_DEATH,
		ENEMY_LAST_BOSS_LEFT_DEATH,

		POINT_MAX
	};

	// ゲームの進行度
	enum STATE_OF_PROGRESS{
		STATE_STAGE01,
		STATE_STAGE02,
		STATE_STAGE03,
		STATE_STAGE04,	// ボス1
		STATE_STAGE05,
		STATE_STAGE06,
		STATE_STAGE07,
		STATE_STAGE08,	// ボス2
		STATE_STAGE09,
		STATE_STAGE10,	// ボス3

		STATE_MAX,
	};

	struct BATTLE_POINT_INFO{
		uint32_t m_scoreUnder;
		uint32_t m_battlePoint;

		void Init(){
			m_scoreUnder = 0;
			m_battlePoint = 0;
		}
	};

	struct STAGE_CLEAR_INFO{
		float		m_userLifeRatio;				// ユーザーライフの残り割合
		uint32_t	m_userScore;					// クリア時のスコア
		uint32_t	m_bonusScore;					// ボーナススコア
		uint32_t	m_hitComboMaxOfStage;			// 各ステージの最大Hit数	
		uint32_t	m_scoreDetail[POINT_MAX];		// スコア詳細
		std::vector< BATTLE_POINT_INFO > m_clearPoint;				// クリアー時に貰えるバトルポイント

		void Init(){
			m_userLifeRatio			= 0.0f;
			m_userScore				= 0;
			m_bonusScore			= 0;
			m_hitComboMaxOfStage	= 0;
			for( uint32_t i = 0; i < NUMBEROF(m_scoreDetail) ;++i ){
				m_scoreDetail[i] = 0;
			}
			m_clearPoint.clear();
		}
	};

	static GameRecorder *Create();
	static GameRecorder *GetInstance();
	~GameRecorder();

	// 初期化
	void InitRecord();
	
	// 途中からプレイかどうか
	void SetContinueFlag(){ m_isContinuePlay = true; }
	const bool &IsContinue() const{ return m_isContinuePlay; }

	// 全クリアフラグON
	void SetClearAll(){ m_isAllClear = true; }
	const bool &IsClearAll() const{ return m_isAllClear; }

	// ステージクリア時のユーザーライフの割合のセット&取得
	const void SetUserLifeRatio( const float &lifeRatio, const STATE_OF_PROGRESS &stage = STATE_MAX );
	const float GetUserLifeRatio( const STATE_OF_PROGRESS &stage = STATE_MAX ) const;

	// ユーザーが指定ステージをクリアしたかどうか
	const bool IsUserClear( const STATE_OF_PROGRESS &stage = STATE_MAX ) const;

	// スコアの加算と取得 GetScoreは引数なしなら直近のステージスコアを返す
	void ScoreEvent( const SCORE_KIND &kind, const uint32_t &tmpInfo = 0 );
	void AddScoreBonus( uint32_t bonusScore, const STATE_OF_PROGRESS &stage = STATE_MAX );
	const int32_t GetScore( const STATE_OF_PROGRESS &stage = STATE_MAX ) const;
	const int32_t GetScoreBonus( const STATE_OF_PROGRESS &stage = STATE_MAX ) const;
	const int32_t GetStageScore( const STATE_OF_PROGRESS &stage = STATE_MAX ) const;
	const int32_t GetTotalScore() const;

	// アイテム取得数の加算
	const void AddItem( Common::ITEM_KIND kind );
	const uint32_t GetItemCount( Common::ITEM_KIND kind ) const;

	// Hit数のIncと取得
	void IncHitCounter();				// Hit数プラス
	uint32_t GetCurrentHitCounter();	// 現在の連続Hit数取得

	// 各ステージの最大コンボ数取得
	uint32_t GetMaxComboNumOfStage( const STATE_OF_PROGRESS &stage = STATE_MAX ) const;

	// 各ステージクリア時に貰えるバトルポイントの情報
	void SetClearBattlePoint( const uint32_t &score, const uint32_t &point, const STATE_OF_PROGRESS &stage = STATE_MAX );
	uint32_t GetClearBattlePoint( const uint32_t &point, const STATE_OF_PROGRESS &stage = STATE_MAX ) const;

	// 保存されているすべてのシーンの取得ポイントを返す
	uint32_t GetClearBattlePointAll() const;

	// コンボ継続有効時間取得
	uint32_t GetLeftTimeOfCombo() const;

	// ゲーム進行状況の更新と取得
	void SetGameStateOfProgress( STATE_OF_PROGRESS nextState );
	const STATE_OF_PROGRESS &GetGameStateOfProgress() const;

protected:
	
	virtual void Update() override;

private:

	GameRecorder();

	static GameRecorder *s_pInstance;

	bool				m_isContinuePlay;				// 途中からのプレイかどうか
	bool				m_isAllClear;					// 全クリアフラグ
	STATE_OF_PROGRESS	m_gameState;					// ゲーム進行度
	STAGE_CLEAR_INFO	m_clearStageInfo[STATE_MAX];	// 各ステージクリア時の情報
	
	// ユーザーの取得アイテム(ゲーム一周での成長率)
	uint32_t			m_getItem[Common::ITEM_KIND_MAX];

	// 以下、ステージクリア毎にクリアされる
	// 連続HIT数
	uint32_t			m_hitComboNum;					// 現在の連続Hit数
	uint32_t			m_hitFailTime_ms;				// 次の連続Hitが有効になる時間(100~30)
};
#endif