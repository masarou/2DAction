/* ====================================================================== */
/**
 * @brief  SystemCollisionManager.h
 *
 * @note	�����蔻����ꊇ�ōs���N���X
 *		
 */
/* ====================================================================== */

#ifndef __SYSTEM_COLLISION_MANAGER__
#define __SYSTEM_COLLISION_MANAGER__

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

	// ���������Փ˒ʒm������K�v�����邩�ǂ����`�F�b�N
	bool NeedEvent( const Common::TYPE_OBJECT typeA, const Common::TYPE_OBJECT typeB ) const;

	CollisionManager(void);
	~CollisionManager(void);

	static CollisionManager	*s_pInstance;

	std::vector<Collision2DUnit*>	m_vCollisionUnit;
};

#endif