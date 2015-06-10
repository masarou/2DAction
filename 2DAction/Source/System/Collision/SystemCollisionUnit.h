/* ====================================================================== */
/**
 * @brief  SystemCollisionUnit.h
 *
 * @note	当たり判定を行う2Dクラスはこれを基底として画像を表示する
 *			SystemMessageUnitを継承しており、当たり判定はEventUpdateで取得
 */
/* ====================================================================== */

#ifndef __SYSTEM_COLLISIONUNIT__
#define __SYSTEM_COLLISIONUNIT__

#include "Game/Game2DBase.h"
#include "System/Message/SystemMessageUnit.h"

class Collision2DUnit : public SystemMessageUnit
{
public:

	// 自分が何者なのかを示すタイプ
	virtual const Common::TYPE_OBJECT GetTypeObject() const = 0 ;

	// CollisionManagerから呼ばれる更新関数
	//void ListUpdate();

	// 当たり判定をするかどうかフラグセット
	void SetInvalidCollisionFlag( const bool &flg ){ m_invalidCollision = flg; }
	const bool GetInvalidCollisionFlag() const{ return m_invalidCollision; }

	// 描画情報取得
	const TEX_DRAW_INFO &GetDrawInfo() const;

	// 当たり判定周りの情報取得
	const uint32_t &GetBelongLv() const;
	const uint32_t &GetBelongIndex() const;

	// 双方向リストに指定クラスを登録
	void ClearChainList(){ m_nextUnit = NULL; m_prevUnit = NULL; }
	Collision2DUnit *GetNextUnit(){ return m_nextUnit; }
	Collision2DUnit *GetPrevUnit(){ return m_prevUnit; }
	void SetNextUnit( Collision2DUnit *pUnit );
	void SetPrevUnit( Collision2DUnit *pUnit );

protected:
	
	Collision2DUnit( const char *readFile = NULL );
	virtual ~Collision2DUnit(void);
	Texture2D			m_drawTexture;	// 表示画像データ

private:

	// 判定をするかどうかフラグ
	bool				m_invalidCollision;

	// 当たり判定で使用する双方向リスト
	Collision2DUnit		*m_nextUnit;
	Collision2DUnit		*m_prevUnit;

	// 直前の空間情報
	uint32_t			m_preBelongLv;
	uint32_t			m_preBelongIndex;
};

#endif