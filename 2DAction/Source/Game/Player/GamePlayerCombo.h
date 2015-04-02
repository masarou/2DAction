/* ====================================================================== */
/**
 * @brief  プレイヤーのコンボ表示を行う
 *
 * @note
 *		
 */
/* ====================================================================== */

#ifndef __GAME_PLAYER_COMBO__
#define __GAME_PLAYER_COMBO__

#include "Common/CommonDefine.h"
#include "Common/CmnNumberCounter.h"
#include "Game/Game2DBase.h"
#include "Game/Item/ItemObject.h"
#include "System/Task/SystemTaskUnit.h"
#include "System/Collision/SystemCollisionUnit.h"

class PlayerCombo : public TaskUnit
{

public:

	static PlayerCombo *CreatePlayerCombo();
	~PlayerCombo(void);

	void AddItemLevel( const Common::ITEM_KIND $kind );
	const uint32_t GetItemLevel( const Common::ITEM_KIND &kind ) const;

	void SetPlayerHP( const uint32_t &val, const uint32_t &lifeMax );

protected:

	virtual bool Init() override;
	virtual bool DieMain() override;
	virtual void Update() override;			// 移動等の内部数値の更新
	virtual void DrawUpdate() override;		// 描画更新

private:
	
	PlayerCombo();
	// パーツ情報セット
	void SetupPartsInfo();
	// 指定パーツの情報取得
	const math::Vector2 GetPartsPos( const std::string name ) const;
	const Common::PARTS_INFO &GetPartsInfo( const std::string name ) const;


	// 描画
	Texture2D		m_textureFrame;				// コンボを表示する外枠
	Texture2D		m_textureCombo;				// 「コンボ」文字列
	Texture2D		m_textureComboGauge;		// コンボゲージ
	NumberCounter	*m_pNumCounterCombo;		// コンボ数(通常)
	NumberCounter	*m_pNumCounterComboYellow;	// コンボ数(黄)
	NumberCounter	*m_pNumCounterComboRed;		// コンボ数(赤)

	// パーツマップ
	std::map< std::string, Common::PARTS_INFO >	m_partsMap;
};

#endif
