/* ====================================================================== */
/**
 * @brief  フィールドに無作為に出てくるアイテムクラス
 *
 * @note
 *		
 */
/* ====================================================================== */

#ifndef __ITEM_OBJECT__
#define __ITEM_OBJECT__

#include "Game/Game2DBase.h"
#include "System/Task/SystemTaskUnit.h"
#include "System/Collision/SystemCollisionUnit.h"

class ItemObject : public TaskUnit, public Collision2DUnit
{
public:

	// 固定値
	static const uint32_t ITEM_LIVE_TIME = 900;	// アイテム存在時間

	enum ITEM_KIND{
		ITEM_KIND_RAPID_BULLET,	// 連射速度UP
		ITEM_KIND_LIFE_UP,		// ライフ回復
		ITEM_KIND_DAMAGE_UP,	// 弾の威力UP

		ITEM_KIND_MAX,
	};
	
	static ItemObject *Create( const ITEM_KIND &kind, math::Vector2 pos = DEFAULT_VECTOR2 );
	virtual ~ItemObject(void);

	// 情報取得
	const TEX_DRAW_INFO &GetDrawInfo() const;
	const uint32_t	&GetLiveTime() const{ return m_liveTime; }
	const ITEM_KIND	&GetItemKind() const{ return m_kindItem; }

protected:

	virtual bool Init() override;									// 初期化
	virtual bool DieMain() override;								// 派生先での初期化
	virtual void Update() override;									// 位置やAIによる数値周りの更新
	virtual void DrawUpdate() override;								// 描画更新
	virtual const Common::TYPE_OBJECT GetTypeObject() const;		// 自分が何者なのかを示すタイプ
	virtual void EventUpdate( const Common::CMN_EVENT &eventId );	// イベント処理関数

private:
	
	ItemObject( const ITEM_KIND &kind, math::Vector2 pos );
	std::string		GetItemFilePath();
 
	ITEM_KIND		m_kindItem;		// アイテムの種類
	uint32_t		m_liveTime;		// 生成されてからの時間
};

#endif
