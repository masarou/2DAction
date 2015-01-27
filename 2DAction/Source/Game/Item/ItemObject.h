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
#include "System/Task/SystemTaskUnit.h"

class ItemObject : public TaskUnit
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
	
	static ItemObject *Create( const ITEM_KIND &kind, const uint32_t &uniqueID, math::Vector2 pos = DEFAULT_VECTOR2 );
	virtual ~ItemObject(void);

	void	SetPlayerGetFlag();	// アイテムの無効化(消滅)

	// 情報取得
	const TEX_DRAW_INFO &GetDrawInfo() const;
	const uint32_t	&GetUniqueNumber() const{ return m_uniqueNumber; }
	const uint32_t	&GetLiveTime() const{ return m_liveTime; }
	const ITEM_KIND	&GetItemKind() const{ return m_kindItem; }
	const bool		&GetPlayerGetFlag() const{ return m_isPlayerGet; }

protected:

	virtual bool Init() override;						// 初期化
	virtual bool DieMain() override;					// 派生先での初期化
	virtual void Update() override;						// 位置やAIによる数値周りの更新
	//virtual void CollisionUpdate() override{};			// 内部数値の更新を受けての他クラスとの当たり判定処理
	virtual void DrawUpdate() override;					// 描画更新

private:
	
	ItemObject( const ITEM_KIND &kind, const uint32_t &uniqueId, math::Vector2 pos );
	std::string		GetItemFilePath();

	bool			m_isPlayerGet;	// 
	ITEM_KIND		m_kindItem;		// アイテムの種類
	uint32_t		m_uniqueNumber;	// ほかのアイテムと区別するためにユニーク番号
	uint32_t		m_liveTime;		// 生成されてからの時間
	Texture2D		m_textureItem;	// タイトル一枚絵
};

#endif
