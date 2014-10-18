/* ====================================================================== */
/**
 * @brief  フィールドに無作為に出てくるアイテムクラス
 *
 * @note
 *		
 */
/* ====================================================================== */

#ifndef ITEM_OBJECT
#define ITEM_OBJECT

#include "Game/Game2DBase.h"

class ItemObject
{
public:

	// 固定値
	static const uint32_t ITEM_LIVE_TIME = 900;	// アイテム存在時間

	enum ITEM_KIND{
		ITEM_RAPID_BULLET,	// 連射速度UP
		ITEM_LIFE_UP,		// ライフ回復
		ITEM_DAMAGE_UP,		// 弾の威力UP

		ITEM_MAX,
	};

public:

	ItemObject( const ITEM_KIND &kind, const uint32_t &uniqueId, const math::Vector2 &pos );
	virtual ~ItemObject(void);
	
	void	Update();
	void	Draw();

	// 情報取得
	const TEX_DRAW_INFO &GetDrawInfo();
	uint32_t GetUniqueNumber(){ return m_uniqueNumber; }
	uint32_t GetLiveTime(){ return m_liveTime; }

private:

	std::string		GetItemFilePath();

	ITEM_KIND		m_kindItem;		// アイテムの種類
	uint32_t		m_uniqueNumber;	// ほかのアイテムと区別するためにユニーク番号
	uint32_t		m_liveTime;		// 生成されてからの時間
	Game2DBase		*m_drawItem;	// アイテム描画クラス
	TEX_DRAW_INFO	m_itemInfo;		// アイテム描画情報

};

#endif
