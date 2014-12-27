/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		ゲーム画面管理クラス
 */
/* ====================================================================== */
#ifndef FLOW_GAME
#define FLOW_GAME

#include "FlowBase.h"

class FlowGame : public FlowBase
{

public:

	static FlowBase *Create( const std::string &fileName );

private:

	virtual bool Init() override;
	virtual void UpdateFlow() override;

	FlowGame( const std::string &fileName );
	virtual ~FlowGame(void);

	void LoadGameSettings( const char *jsonFile );

	uint32_t					m_gameTimer;		// ゲームのプレイ時間

	uint32_t					m_gameTimeMax;		// ゲーム終了までの時間(/fps)
	uint32_t					m_enemyMax;			// 最大出現敵数
	uint32_t					m_enemyFrequency;	// 敵の出現率(10段階)
	uint32_t					m_itemMax;			// 最大出現アイテム数
	uint32_t					m_itemFrequency;	// アイテムの出現率(10段階)
};
#endif

