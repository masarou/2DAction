/* ====================================================================== */
/**
 * @brief  敵の生成やアイテム生成を設定にならって管理するクラス
 *
 * @note
 *		
 */
/* ====================================================================== */

#ifndef __GAME_MANAGER__
#define __GAME_MANAGER__

#include "System/Task/SystemTaskUnit.h"

class GameManager : public TaskUnit
{
public:

	enum STAGE_TYPE{
		TYPE_TIME,
		TYPE_DESTROY,

		TYPE_MAX,
	};

	static GameManager *CreateGameManager( const Common::GAME_FLOW &currentKind );
	~GameManager(void);

	// ステージクリア条件を満たしたかどうか
	bool	IsGameOver() const;

	// 敵を倒した数をカウント
	void AddDestroyCount();

	// ステージタイプ取得
	const STAGE_TYPE GetStageType() const;

	// 残り時間or殲滅数の取得
	const uint32_t GetGameLeftTimeByFrame() const;
	const uint32_t GetGameLeftTimeBySec() const;
	const uint32_t GetGameLeftDestroy() const;

protected:

	virtual bool Init() override;
	virtual void Update() override;
	virtual bool DieMain() override;

private:

	GameManager( const Common::GAME_FLOW &currentKind );
	
	void ResetManageValue();										// 初期化
	void LoadGameSettings( const char *jsonFile );					// ゲーム環境の読み込み
	bool IsCreateEnemy( uint32_t enemyLimit, uint32_t frequency );	// 敵の生成判断
	bool IsCreateItem( uint32_t itemLimit, uint32_t frequency );	// アイテムの生成判断

	STAGE_TYPE		m_type;				// ステージクリアのタイプ

	// ステージが制限時間をもつものだった場合
	uint32_t		m_gameTimer;		// ゲームのプレイ時間
	uint32_t		m_gameTimeMax;		// ゲーム終了までの時間(/fps)

	// ステージが敵殲滅数をクリア条件とする場合
	uint32_t		m_destroyNum;		// 現在までの敵殲滅数
	uint32_t		m_destroyMax;		// ステージ終了までの敵殲滅数

	// ステージ環境周り変数
	uint32_t		m_enemyMax;			// 最大出現敵数
	uint32_t		m_enemyFrequency;	// 敵の出現率(10段階0~9)
	uint32_t		m_itemMax;			// 最大出現アイテム数
	uint32_t		m_itemFrequency;	// アイテムの出現率(10段階0~9)
	std::string		m_settingFileStr;	// 読み込むステージ設定ファイル
};

#endif