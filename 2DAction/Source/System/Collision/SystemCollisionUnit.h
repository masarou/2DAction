/* ====================================================================== */
/**
 * @brief  SystemCollisionUnit.h
 *
 * @note	当たり判定を行う2Dクラスはこれを基底として画像を表示する
 *			SystemMessageUnitを継承しており、当たり判定はEventUpdateで取得
 */
/* ====================================================================== */

#ifndef SYSTEM_COLLISIONUNIT
#define SYSTEM_COLLISIONUNIT

#include "System/Message/SystemMessageUnit.h"
#include "Game/Game2DBase.h"

class Collision2DUnit : public SystemMessageUnit
{
public:

	// 自分が何者なのかを示すタイプ
	virtual const Common::TYPE_OBJECT GetTypeObject() const = 0 ;

	// 描画情報取得
	const TEX_DRAW_INFO &GetDrawInfo() const;

protected:
	
	Collision2DUnit( const char *readFile = NULL );
	virtual ~Collision2DUnit(void);
	Texture2D			m_drawTexture;	// 表示画像データ

private:


};

#endif