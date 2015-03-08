/* ====================================================================== */
/**
 * @brief  SystemCollisionManager.h
 *
 * @note	当たり判定を一括で行うクラス
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

	// 管理ユニットの登録と削除
	void AddUnit( Collision2DUnit *pUnit );
	void RemoveUnit( Collision2DUnit *pUnit );

	// 登録ユニットの当たり判定開始
	void CollisionUpdate();

	// このクラスの解放
	static void DeleteCollisionManager();
	
	// 所属Lvとその空間のIndex番号から「m_objectTree」の配列番号を求める
	uint32_t GetRegisterTreeIndex( const Collision2DUnit *pUnit ) const;
	uint32_t GetRegisterTreeIndex( const uint32_t &belongLv, const uint32_t &belongIndex ) const;

	// 指定クラスを「m_belongAreaTopClass」に登録
	void RegisterUnitFromTree( const uint32_t &treeIndex, Collision2DUnit *pUnit );
	void RemoveUnitFromTree( const uint32_t &treeIndex, Collision2DUnit *pUnit );

private:

	struct TREE_INFO{
		uint32_t registNum;		// いくつのCollisionUnitが連結しているか
		Collision2DUnit *pUnit;	// 連結CollisionUnitの先頭
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

	// そもそも衝突通知をする必要があるかどうかチェック
	bool NeedEvent( const Common::TYPE_OBJECT typeA, const Common::TYPE_OBJECT typeB ) const;

	// 同じエリアのオブジェクトについて再帰的に当たり判定を行う
	void RecursiveSameAreaCheck( Collision2DUnit *pUnit );

	// 当たり判定を実際に行う
	void CheckUnitCollision( Collision2DUnit *unitA, Collision2DUnit *unitB );

	CollisionManager(void);
	~CollisionManager(void);

	static CollisionManager	*s_pInstance;

	std::vector<Collision2DUnit*>	m_vCollisionUnit;

	// 当たり判定を行う際、各所属空間の先頭Collisionを格納した配列
	std::vector<Collision2DUnit*>	m_vCheckCollision;
	TREE_INFO m_objectTree[4095];
};

#endif