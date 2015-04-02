/* ====================================================================== */
/**
 * @brief  プレイヤーのステータス一括表示画面
 *
 * @note
 *		
 */
/* ====================================================================== */

#ifndef __GAME_PLAYER_STATUS_MENU__
#define __GAME_PLAYER_STATUS_MENU__

#include "Common/CommonDefine.h"
#include "Game/Game2DBase.h"
#include "Game/Item/ItemObject.h"
#include "System/Task/SystemTaskUnit.h"
#include "System/Collision/SystemCollisionUnit.h"

class PlayerStatusMenu : public TaskUnit
{

public:

	PlayerStatusMenu();
	~PlayerStatusMenu(void);

	void AddItemLevel( const Common::ITEM_KIND $kind );
	const uint32_t GetItemLevel( const Common::ITEM_KIND &kind ) const;

	void SetPlayerHP( const uint32_t &val, const uint32_t &lifeMax );

protected:

	virtual bool Init() override;
	virtual bool DieMain() override;
	virtual void Update() override;			// 移動等の内部数値の更新
	virtual void DrawUpdate() override;		// 描画更新

private:

	// 指定パーツの情報取得
	const math::Vector2 GetPartsPos( const std::string name ) const;
	const Common::PARTS_INFO &GetPartsInfo( const std::string name ) const;

	// 描画
	Texture2D		m_textureStatus;		// ステータス画像
	Texture2D		m_lifeGauge;			// ライフゲージ
	Texture2D		m_danageLevel;			// 攻撃力レベル数字
	Texture2D		m_rapidLevel;			// 連射速度レベル数字

	// プレイヤーライフ
	uint32_t		m_playerLife;
	uint32_t		m_playerLifeMax;

	// アイテムレベル
	uint32_t		m_itemLevelDamage;		// 威力レベル
	uint32_t		m_itemLevelRapid;		// 連射レベル

	// パーツマップ
	std::map< std::string, Common::PARTS_INFO >	m_partsMap;
};

#endif
