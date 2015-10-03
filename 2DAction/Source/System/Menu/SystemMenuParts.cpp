/* ====================================================================== */
/**
 * @brief  各画面パーツクラス
 *
 * @note
 *		
 */
/* ====================================================================== */


#include "SystemMenuParts.h"
#include "SystemMenuPartsFactory.h"
#include "System/Draw2D/SystemDraw2DResource.h"
#include "Common/Utility/CommonGameUtility.h"

MenuParts *MenuParts::Create( const std::string &partsStr, const std::string &jsonStr, const Common::PRIORITY &priority, const math::Vector2 &originalPos )
{
	return NEW MenuParts( partsStr, jsonStr, priority, originalPos );
}

MenuParts::MenuParts( const std::string &partsStr, const std::string &jsonStr, const Common::PRIORITY &priority, const math::Vector2 &originalPos )
: m_invalidDraw( false )
, m_partsNameStr( partsStr )
, m_readJsonStr( jsonStr )
, m_originPos( originalPos )
, m_priority( priority )
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
 * @brief	画面パーツの更新トリガー 子パーツも含め更新指示
 */
/* ================================================ */
void MenuParts::UpdatePartsRecursive()
{
	UpdateParts();

	for( uint32_t i = 0; i < m_partsArray.size() ; ++i ){
		m_partsArray.at(i)->UpdatePartsRecursive();
	}
}

/* ================================================ */
/**
 * @brief	画面パーツの描画トリガー 子パーツも含め描画指示
 */
/* ================================================ */
void MenuParts::DrawPartsRecursive()
{
	if( !m_invalidDraw && m_texMine.m_pTex2D ){
		m_texMine.m_pTex2D->DrawUpdate2D();
	}

	for( uint32_t i = 0; i < m_partsArray.size() ; ++i ){
		m_partsArray.at(i)->DrawPartsRecursive();
	}
}

/* ================================================ */
/**
 * @brief	子も含めてパーツ削除
 */
/* ================================================ */
MenuParts *MenuParts::GetPartsRecursive( const std::string &partsStr )
{
	// 探しているのが自分ではないかチェック
	if( m_partsNameStr.compare( partsStr ) == 0 ){
		return this;
	}

	// 自分でなければ子クラスを探す
	for( uint32_t i = 0; i < m_partsArray.size() ; ++i ){
		MenuParts *pSearchResult = m_partsArray.at(i)->GetPartsRecursive( partsStr );
		if( pSearchResult ){
			return pSearchResult;
		}
	}

	// 見つからなかった
	return NULL;
}


/* ================================================ */
/**
 * @brief	描画するかどうかフラグセット
 */
/* ================================================ */
void MenuParts::SetDrawFlag( const bool &isDraw )
{
	m_invalidDraw = !isDraw;
	
	// 子クラスにも伝達
	for( uint32_t i = 0; i < m_partsArray.size() ; ++i ){
		m_partsArray.at( i )->SetDrawFlag( isDraw );
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
	// ステータスメニューのパーツ情報取得
	Utility::GetPartsInfoFromJson( m_readJsonStr, m_partsMap );
	auto it = m_partsMap.begin();
	for(;;)
	{
		if( it == m_partsMap.end() ){
			break;
		}
		MenuParts *parts = CreatePartsFactory( it->second.m_type, it->first, it->second.m_jsonStr, m_priority, it->second.m_pos + m_originPos );
		m_partsArray.push_back(parts);
		++it;
	}

	if( m_partsNameStr.compare("root") != 0 || m_partsArray.empty() ){
		// root(一番上)でなければ自身のパーツセット
		// 作られないこともあるのでNULLチェックも行う
		m_texMine.m_pTex2D = Game2DBase::CreateWithCheck( m_readJsonStr.c_str() );
		if( m_texMine.m_pTex2D ){
			TEX_DRAW_INFO drawInfo;
			drawInfo.m_fileName = m_readJsonStr.c_str();
			drawInfo.m_posOrigin.x	= m_originPos.x;
			drawInfo.m_posOrigin.y	= m_originPos.y;
			drawInfo.m_prioity		= m_priority;
			drawInfo.m_usePlayerOffset = false;
			m_texMine.m_pTex2D->SetDrawInfo( drawInfo );
		}
	}



}

