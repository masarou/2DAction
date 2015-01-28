/* ====================================================================== */
/**
 * @brief  �G�Ǘ��N���X
 *
 * @note
 *		
 */
/* ====================================================================== */

#ifndef ENEMY_MANAGER
#define ENEMY_MANAGER

#include "System/SystemDefine.h"
#include "System/Task/SystemTaskUnit.h"
#include "System/Message/SystemMessageUnit.h"
#include "System/Draw2D/SystemDraw2DDefine.h"

class GamePlayer;
class Bullet;
class EnemyBase;

class EnemyManager : public TaskUnit
{

public:

	static EnemyManager *CreateEnemyManager();

	void AddEnemy( const Common::ENEMY_KIND &kind );				// �G�L��������
	void RemoveEnemy( EnemyBase *removeEnemy );						// �w��N���X���Ǘ�����O��

	// �w��̓G�̐��𐔂���
	uint32_t CountEnemy( const Common::ENEMY_KIND &kind = Common::ENEMY_KIND_MAX ) const;

protected:
	
	virtual bool DieMain() override;

private:

	EnemyManager(void);
	~EnemyManager(void);

	std::vector<EnemyBase*> m_enemyArray;	// �G�Ǘ�
};

#endif

