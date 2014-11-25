/* ====================================================================== */
/**
 * @brief  �v���C���[�A�G�Ȃǂ̃��j�[�N�N���X������
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

	void DeleteRegister();	// ���W�X�^���

private:

	GameRegister(void);
	~GameRegister(void);


	static GameRegister *s_pInstance;

	// �ȉ��̃N���X�̉����TaskUnit�ɔC����̂Ő����݂̂ł悢
	GamePlayer			*m_pPlayer;
	EnemyManager		*m_pEnemyManager;
	GameMap				*m_pMap;
	ItemManager			*m_pItemManager;

};

#endif
