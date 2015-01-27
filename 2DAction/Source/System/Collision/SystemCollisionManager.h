/* ====================================================================== */
/**
 * @brief  SystemCollisionManager.h
 *
 * @note	当たり判定を一括で行うクラス
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

	// 管理ユニットの登録と削除
	void AddUnit( Collision2DUnit *pUnit );
	void RemoveUnit( Collision2DUnit *pUnit );

	// 登録ユニットの当たり判定開始
	void CollisionUpdate();

	// このクラスの解放
	static void DeleteCollisionManager();

private:

	CollisionManager(void);
	~CollisionManager(void);

	static CollisionManager	*s_pInstance;

	std::vector<Collision2DUnit*>	m_vCollisionUnit;
};

#endif