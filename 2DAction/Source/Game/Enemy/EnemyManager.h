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

#include "System/Task/SystemTaskUnit.h"
#include "System/SystemDefine.h"
#include "EnemyBase.h"

class GamePlayer;
class Bullet;

class EnemyManager : TaskUnit
{

public:

	static EnemyManager *CreateEnemyManager();

	void CreateEnemy( const Common::ENEMY_KIND &kind );		// �G�L��������
	void DeleteEnemy( const uint32_t &uniqueNumber );		// �w��N���X���Ǘ�����O��

	bool CheckCollision( const TEX_DRAW_INFO &texInfo );	// ��������̕`�敨���G�ɓ������Ă��邩�ǂ����`�F�b�N
	bool CheckCollisionToBullet( Bullet *bullet );			// �v���C���[�̒e�Ƃ̓����蔻��
	bool CheckCollisionToPlayer( GamePlayer *player );		// �v���C���[�Ƃ̓����蔻��

protected:
	
	virtual void Update() override;
	virtual void CollisionUpdate() override;
	virtual void DrawUpdate() override;

	// ���b�Z�[�W����
	void EventUpdate( const Common::CMN_EVENT &eventId ) override;

private:

	EnemyManager(void);
	~EnemyManager(void);
	
	std::vector<EnemyBase*> m_enemyArray;	// �G�Ǘ�

};

#endif

