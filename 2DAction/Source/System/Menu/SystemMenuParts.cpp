/* ====================================================================== */
/**
 * @brief  各画面パーツクラス
 *
 * @note
 *		
 */
/* ====================================================================== */


#include "SystemMenuParts.h"
#include "System/Draw2D/SystemDraw2DResource.h"
#include "Common/Utility/CommonGameUtility.h"

MenuParts *MenuParts::Create( const std::string &partsStr, const std::string &jsonStr, const math::Vector2 &originalPos )
{
	return NEW MenuParts( partsStr, jsonStr, originalPos );
}

MenuParts::MenuParts( const std::string &partsStr, const std::string &jsonStr, const math::Vector2 &originalPos )
: m_partsNameStr( partsStr )
, m_readJsonStr( jsonStr )
, m_originPos( originalPos )
{
	m_texMine.Init();
	m_partsArray.clear();
	m_partsMap.clear();

	// ここでパーツのすべてをセットアップ!!
	SetupParts();

}


MenuParts::~MenuParts(void)
{
}

/* ================================================ */
/**
 * @brief	画面パーツの描画トリガー 子パーツも含め描画指示
 */
/* ================================================ */
void MenuParts::DrawParts()
{
	if( m_texMine.m_pTex2D ){
		m_texMine.m_pTex2D->DrawUpdate2D();
	}

	for( uint32_t i = 0; i < m_partsArray.size() ; ++i ){
		m_partsArray.at(i)->DrawParts();
	}
}

/* ================================================ */
/**
 * @brief	子も含めてパーツ削除
 */
/* ================================================ */
bool MenuParts::DeletePartsAll()
{
	auto it = m_partsArray.begin();
	while( it != m_partsArray.end() ){
		MenuParts *parts = *it;

		// パーツに削除して良いかお伺い
		bool isFinish = parts->DeletePartsAll();
		if( !isFinish ){
			return false;
			//break;
		}

		// 削除準備できたので削除
		it = m_partsArray.erase( it );
		SAFE_DELETE( parts );
	}
	
	// 自分削除
	m_texMine.DeleteAndInit();
	return true;
}

/* ================================================ */
/**
 * @brief	指定パーツ名を探索＋指定ラベルセット再帰関数
 */
/* ================================================ */
bool MenuParts::SetPartsAnim( const std::string &partsName, const std::string &animStr )
{
	if( m_partsNameStr.compare( partsName ) == 0 ){
		m_texMine.m_pTex2D->SetAnim( animStr );
		return true;
	}

	for( uint32_t i = 0; i < m_partsArray.size() ; ++i ){
		if( m_partsArray.at(i)->SetPartsAnim( partsName, animStr ) ){
			return true;
		}
	}
	return false;
}

/* ================================================ */
/**
 * @brief	画面セットアップ準備再帰関数
 */
/* ================================================ */
void MenuParts::SetupParts()
{
	if( m_partsNameStr.compare("root") != 0 ){
		// root(一番上)でなければまず自身のパーツセット
		m_texMine.m_pTex2D = NEW Game2DBase( m_readJsonStr.c_str() );
		m_texMine.m_texInfo.m_fileName = m_readJsonStr.c_str();
		m_texMine.m_texInfo.m_posOrigin.x = m_originPos.x;
		m_texMine.m_texInfo.m_posOrigin.y = m_originPos.y;
		m_texMine.m_texInfo.m_usePlayerOffset = false;
		m_texMine.m_pTex2D->SetDrawInfo(m_texMine.m_texInfo);
	}

	// ステータスメニューのパーツ情報取得
	Utility::GetPartsInfoFromJson( m_readJsonStr, m_partsMap );
	auto it = m_partsMap.begin();
	for(;;)
	{
		if( it == m_partsMap.end() ){
			break;
		}
		MenuParts *parts = Create( it->first, it->second.m_jsonStr, it->second.m_pos + m_originPos );
		m_partsArray.push_back(parts);
		++it;
	}

}


