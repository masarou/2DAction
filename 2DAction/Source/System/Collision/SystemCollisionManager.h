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
#include "Game/GameMap.h"

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
	
	// ����Lv�Ƃ��̋�Ԃ�Index�ԍ�����um_objectTree�v�̔z��ԍ������߂�
	uint32_t GetRegisterTreeIndex( const Collision2DUnit *pUnit ) const;
	uint32_t GetRegisterTreeIndex( const uint32_t &belongLv, const uint32_t &belongIndex ) const;

	// �w��N���X���um_belongAreaTopClass�v�ɓo�^
	void RegisterUnitFromTree( const uint32_t &treeIndex, Collision2DUnit *pUnit );
	void RemoveUnitFromTree( const uint32_t &treeIndex, Collision2DUnit *pUnit );

private:

	struct TREE_INFO{
		uint32_t registNum;		// ������CollisionUnit���A�����Ă��邩
		Collision2DUnit *pUnit;	// �A��CollisionUnit�̐擪
		void Init(){
			registNum = 0;
			pUnit = NULL;
		}
		void AddUnit( Collision2DUnit *addUnit ){
			if( pUnit != NULL ){
				addUnit->SetNextUnit( pUnit );
			}
			pUnit = addUnit;
			++registNum;
		}
	};

	// ���������Փ˒ʒm������K�v�����邩�ǂ����`�F�b�N
	bool NeedEvent( const Common::TYPE_OBJECT typeA, const Common::TYPE_OBJECT typeB ) const;

	// �����G���A�̃I�u�W�F�N�g�ɂ��čċA�I�ɓ����蔻����s��
	void RecursiveSameAreaCheck( Collision2DUnit *pUnit );

	// �����蔻������ۂɍs��
	void CheckUnitCollision( Collision2DUnit *unitA, Collision2DUnit *unitB );

	CollisionManager(void);
	~CollisionManager(void);

	static CollisionManager	*s_pInstance;

	std::vector<Collision2DUnit*>	m_vCollisionUnit;

	// �����蔻����s���ہA�e������Ԃ̐擪Collision���i�[�����z��
	std::vector<Collision2DUnit*>	m_vCheckCollision;
	TREE_INFO m_objectTree[4095];
};

#endif