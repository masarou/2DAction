/* ====================================================================== */
/**
 * @brief  �G�Ǘ��N���X
 *
 * @note
 *		�G�̐����͕K��GameManager(friend�N���X)��ʂ��čs���܂�
 */
/* ====================================================================== */

#ifndef __ENEMY_MANAGER__
#define __ENEMY_MANAGER__

#include "System/SystemDefine.h"
#include "System/Task/SystemTaskUnit.h"
#include "System/Message/SystemMessageUnit.h"
#include "System/Draw2D/SystemDraw2DDefine.h"
#include "Game/GameManager.h"

class GamePlayer;
class Bullet;
class EnemyBase;

class EnemyManager : public TaskUnit
{
	friend class GameManager;

public:

	static EnemyManager *CreateEnemyManager();

	void RemoveEnemy( EnemyBase *removeEnemy );				// �w��N���X���Ǘ�����O��

	// �w��̓G�̐��𐔂���
	uint32_t CountEnemy( const Common::ENEMY_KIND &kind = Common::ENEMY_KIND_MAX ) const;

protected:
	
	virtual bool DieMain() override;

private:

	void AddEnemy( const Common::ENEMY_KIND &kind, const uint32_t &enemyLevel );		// �G�L��������

	EnemyManager(void);
	~EnemyManager(void);

	std::vector<EnemyBase*> m_enemyArray;	// �G�Ǘ�
};

#endif

