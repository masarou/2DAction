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
	};

	static GameManager *CreateGameManager( const Common::GAME_FLOW &currentKind );

	~GameManager(void);
	// 制限時間を過ぎたかどうか
	bool	IsGameOver() const;

	// 敵を倒した数をカウント
	void AddDestroyCount();

protected:

	virtual bool Init() override;
	virtual void Update() override;
	virtual bool DieMain() override;

private:

	GameManager( const Common::GAME_FLOW &currentKind );
	
	void LoadGameSettings( const char *jsonFile );					// ゲーム環境の読み込み
	bool IsCreateEnemy( uint32_t enemyLimit, uint32_t frequency );	// 敵の生成判断
	bool IsCreateItem( uint32_t itemLimit, uint32_t frequency );	// アイテムの生成判断

	STAGE_TYPE		m_type;				// ステージクリアのタイプ

	uint32_t		m_gameTimer;		// ゲームのプレイ時間
	uint32_t		m_gameTimeMax;		// ゲーム終了までの時間(/fps)

	uint32_t		m_destroyNum;		// 現在までの敵殲滅数
	uint32_t		m_destroyMax;		// ゲーム終了までの敵殲滅数

	uint32_t		m_enemyMax;			// 最大出現敵数
	uint32_t		m_enemyFrequency;	// 敵の出現率(10段階0~9)
	uint32_t		m_itemMax;			// 最大出現アイテム数
	uint32_t		m_itemFrequency;	// アイテムの出現率(10段階0~9)
	std::string		m_settingFileStr;	// 読み込むステージ設定ファイル
};

#endif