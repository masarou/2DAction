/* ====================================================================== */
/**
 * @brief  �v���C���[�A�G�Ȃǂ̃��j�[�N�N���X������
 *
 * @note
 *		
 */
/* ====================================================================== */

#ifndef __SYSTEM_GAME_REGISTER__
#define __SYSTEM_GAME_REGISTER__

#include "Common/CommonDefine.h"
#include "Common/CmnGameAccesser.h"
#include "Game/Player/GamePlayer.h"
#include "Game/Player/GamePlayerCombo.h"
#include "Game/Enemy/EnemyManager.h"
#include "Game/Item/ItemManager.h"
#include "Game/GameMap.h"
#include "Game/GameManager.h"

class GameRegister
{
	friend GamePlayer;
	friend EnemyManager;

public:

	static GameRegister *CreateInstance();
	static GameRegister *GetInstance();

	GamePlayer			*UpdatePlayer();
	EnemyManager		*UpdateManagerEnemy();
	GameMap				*UpdateGameMap();
	ItemManager			*UpdateManagerItem();
	GameManager			*UpdateManagerGame();

	const GamePlayer		*GetPlayer() const;
	const EnemyManager		*GetManagerEnemy() const;
	const GameMap			*GetGameMap() const;
	const ItemManager		*GetManagerItem() const;
	const GameManager		*GetManagerGame() const;

	void DeleteRegister();	// ���W�X�^���

private:

	GameRegister(void);
	~GameRegister(void);


	static GameRegister *s_pInstance;

	// �ȉ��̃N���X�̉����TaskUnit�ɔC����̂Ő����݂̂ł悢
	GamePlayer			*m_pPlayer;
	PlayerCombo			*m_pPlayerCombo;
	EnemyManager		*m_pEnemyManager;
	GameMap				*m_pMap;
	ItemManager			*m_pItemManager;
	GameManager			*m_pGameManager;

};

#endif
