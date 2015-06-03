/* ====================================================================== */
/**
 * @brief  画面を構成するパーツひとつのクラス
 *
 * @note
 *		
 */
/* ====================================================================== */
#ifndef __SYSTEM_MENU_PARTS__
#define __SYSTEM_MENU_PARTS__

#include "System/SystemDefine.h"
#include "Common/CommonDefine.h"
#include "Game/Game2DBase.h"

class MenuParts
{

public:

	static MenuParts *Create( const std::string &partsStr, const std::string &jsonStr, const math::Vector2 &originalPos );

	MenuParts( const std::string &partsStr, const std::string &jsonStr, const math::Vector2 &originalPos );
	~MenuParts(void);

	// 子パーツクラス含め描画トリガー関数
	void DrawParts();

	// 終了関数
	bool DeletePartsAll();

	// アニメを変えたいときに使用
	bool SetPartsAnim( const std::string &partsName, const std::string &animStr );

protected:


private:

	// パーツのセットアップ再帰関数
	void SetupParts();
	
	std::string									m_partsNameStr;	// 親に名づけられたパーツ名
	std::string									m_readJsonStr;	// 読み込んだjsonファイル名
	math::Vector2								m_originPos;	// 親クラスに指定された位置情報

	Texture2D									m_texMine;		// 自身のテクスチャクラス

	std::vector<MenuParts*>						m_partsArray;	// このクラスのパーツに付随しているパーツ
	std::map< std::string, Common::PARTS_INFO >	m_partsMap;		// 付随パーツの情報
};
#endif // __SYSTEM_MENU_PARTS__