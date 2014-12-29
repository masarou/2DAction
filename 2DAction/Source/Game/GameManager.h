/* ====================================================================== */
/**
 * @brief  敵の生成やアイテム生成を設定にならって管理するクラス
 *
 * @note
 *		
 */
/* ====================================================================== */

#ifndef GAME_MANAGER
#define GAME_MANAGER

#include "System/Task/SystemTaskUnit.h"

class GameManager : public TaskUnit
{
public:

	GameManager(void);
	~GameManager(void);

	static GameManager *CreateGameManager();

	// 制限時間を過ぎたかどうか
	bool	IsGameOver() const;

protected:

	virtual bool Init() override;
	virtual void Update() override;
	virtual bool DieMain() override;

private:

	
	void LoadGameSettings( const char *jsonFile );					// ゲーム環境の読み込み
	bool IsCreateEnemy( uint32_t enemyLimit, uint32_t frequency );	// 敵の生成判断
	bool IsCreateItem( uint32_t itemLimit, uint32_t frequency );	// アイテムの生成判断

	uint32_t		m_gameTimer;		// ゲームのプレイ時間

	uint32_t		m_gameTimeMax;		// ゲーム終了までの時間(/fps)
	uint32_t		m_enemyMax;			// 最大出現敵数
	uint32_t		m_enemyFrequency;	// 敵の出現率(10段階0~9)
	uint32_t		m_itemMax;			// 最大出現アイテム数
	uint32_t		m_itemFrequency;	// アイテムの出現率(10段階0~9)

};

#endif