/* ====================================================================== */
/**
 * @brief  プレイヤー、敵などのユニーククラスを持つ
 *
 * @note
 *		
 */
/* ====================================================================== */

#ifndef SYSTEM_GAME_REGISTER
#define SYSTEM_GAME_REGISTER

#include "Common/CommonDefine.h"
#include "Common/CmnGameAccesser.h"
#include "Game/Player/GamePlayer.h"
#include "Game/Enemy/EnemyManager.h"
#include "Game/Item/ItemManager.h"
#include "Game/GameMap.h"

class GameRegister
{
	friend GamePlayer;
	friend EnemyManager;

public:

	static GameRegister *CreateInstance();
	static GameRegister *GetInstance();

	GamePlayer			*GetPlayer();
	EnemyManager		*GetManagerEnemy();
	GameMap				*GetGameMap();
	ItemManager			*GetManagerItem();

	void DeleteRegister();	// レジスタ解放

private:

	GameRegister(void);
	~GameRegister(void);


	static GameRegister *s_pInstance;

	// 以下のクラスの解放はTaskUnitに任せるので生成のみでよい
	GamePlayer			*m_pPlayer;
	EnemyManager		*m_pEnemyManager;
	GameMap				*m_pMap;
	ItemManager			*m_pItemManager;

};

#endif
