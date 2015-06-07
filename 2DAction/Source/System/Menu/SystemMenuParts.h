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

	~MenuParts(void);

	// 終了関数
	bool DeletePartsAll();

	// アニメを変えたいときに使用
	bool SetPartsAnim( const std::string &partsName, const std::string &animStr );

	// 描画情報取得＆更新
	TEX_DRAW_INFO &GetTexDrawInfo(){ return m_texMine.m_texInfo; }

	// 各派生先で必要になるUpdate
	void UpdatePartsRecursive();

	// 子も含め描画トリガー関数
	void DrawPartsRecursive();

	// 指定されたパーツクラスを再帰的に探すなければNULL
	MenuParts *GetPartsRecursive( const std::string &partsStr );

	// 描画するかどうかフラグ
	void SetDrawFlag( const bool &isDraw );

	// 以下、派生先で要実装の可能性がある関数
	// パーツタイプ取得
	virtual Common::PARTS_TYPE_MENU GetPartsType(){ return Common::PARTS_SINGLE_DRAW; }
	virtual void UpdateParts(){};

protected:

	MenuParts( const std::string &partsStr, const std::string &jsonStr, const math::Vector2 &originalPos );

private:

	// パーツのセットアップ再帰関数
	void SetupParts();

protected:
	
	bool										m_invalidDraw;	// 描画するかどうかフラグ(立ってたら描画しない)
	std::string									m_partsNameStr;	// 親に名づけられたパーツ名
	std::string									m_readJsonStr;	// 読み込んだjsonファイル名
	math::Vector2								m_originPos;	// 親クラスに指定された位置情報

	Texture2D									m_texMine;		// 自身のテクスチャクラス

	std::vector<MenuParts*>						m_partsArray;	// このクラスのパーツに付随しているパーツ
	std::map< std::string, Common::PARTS_INFO >	m_partsMap;		// 付随パーツの情報
};

#endif // __SYSTEM_MENU_PARTS__