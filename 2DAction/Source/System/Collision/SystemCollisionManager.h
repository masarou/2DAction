/* ====================================================================== */
/**
 * @brief  SystemCollisionManager.h
 *
 * @note	�����蔻����ꊇ�ōs���N���X
 *		
 */
/* ====================================================================== */

#ifndef SYSTEM_COLLISION_MANAGER
#define SYSTEM_COLLISION_MANAGER

#include "System/SystemDefine.h"
#include "SystemCollisionUnit.h"

class CollisionManager
{
public:

	static CollisionManager *Create();
	static CollisionManager *GetInstance();

	// �Ǘ����j�b�g�̓o�^�ƍ폜
	void AddUnit( Collision2DUnit *pUnit );
	void RemoveUnit( Collision2DUnit *pUnit );

	// �o�^���j�b�g�̓����蔻��J�n
	void CollisionUpdate();

	// ���̃N���X�̉��
	static void DeleteCollisionManager();

private:

	CollisionManager(void);
	~CollisionManager(void);

	static CollisionManager	*s_pInstance;

	std::vector<Collision2DUnit*>	m_vCollisionUnit;
};

#endif