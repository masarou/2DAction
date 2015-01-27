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

	uint32_t CountEnemy() const{ return m_enemyArray.size(); }

protected:
	
	virtual void CollisionUpdate() override;
	virtual bool DieMain() override;

private:

	EnemyManager(void);
	~EnemyManager(void);

	bool CheckCollision( const TEX_DRAW_INFO &texInfo ) const;		// ��������̕`�敨���G�ɓ������Ă��邩�ǂ����`�F�b�N
	//bool CheckCollisionToBullet( const Bullet *bullet );			// �v���C���[�̒e�Ƃ̓����蔻��
	//bool CheckCollisionToPlayer( const GamePlayer *player ) const;	// �v���C���[�Ƃ̓����蔻��

	std::vector<EnemyBase*> m_enemyArray;	// �G�Ǘ�
};

#endif

