/* ====================================================================== */
/**
 * @brief  �G�Ǘ��N���X
 *
 * @note
 *		
 */
/* ====================================================================== */

#include "System/Task/SystemTaskUnit.h"
#include "System/SystemDefine.h"
#include "EnemyBase.h"

class Bullet;

class EnemyManager : TaskUnit
{

public:

	static EnemyManager *GetInstance();
	void CreateEnemy( Common::ENEMY_KIND kind );	// �G�L��������
	void DeleteEnemy( uint32_t uniqueNumber );		// �w��N���X���Ǘ�����O��
	void CheckCollision( Bullet *bullet );			// �v���C���[�̒e�Ƃ̓����蔻��

protected:
	
	virtual void Update() override;
	virtual void DrawUpdate() override;

	// ���b�Z�[�W����
	void EventUpdate( const Common::CMN_EVENT &eventId ) override;

private:

	EnemyManager(void);
	~EnemyManager(void);
	
	static EnemyManager		*m_pInstance;
	std::vector<EnemyBase*> m_enemyArray;	// �G�Ǘ�

};

