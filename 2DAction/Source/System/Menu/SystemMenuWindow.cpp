/* ====================================================================== */
/**
 * @brief  背景管理クラス
 *
 * @note
 *		
 */
/* ====================================================================== */


#include "SystemMenuWindow.h"
#include "System/Draw2D/SystemDraw2DResource.h"
#include "System/Menu/SystemMenuParts.h"
#include "Common/Utility/CommonGameUtility.h"

MenuWindow::MenuWindow( const std::string &readMenuJson )
: TaskUnit("MenuWindow")
, m_pMainParts( NULL )
, m_readMenuFile( readMenuJson )
{
}


MenuWindow::~MenuWindow(void)
{
}

void MenuWindow::SetAnim( const std::string &partsStr, const std::string &animStr )
{
	if( m_pMainParts ){
		m_pMainParts->SetPartsAnim( partsStr, animStr );
	}
}

MenuParts	*MenuWindow::GetParts( const std::string &partsStr )
{
	if( m_pMainParts ){
		MenuParts *pParts = m_pMainParts->GetPartsRecursive( partsStr );
		if( pParts ){
			return pParts;
		}
	}
	DEBUG_ASSERT( 0, "指定されたMenuパーツは見つかりませんでした");
	return NULL;
}

PartsCounter *MenuWindow::GetPartsCounter( const std::string &partsStr )
{
	MenuParts *pParts = GetParts( partsStr );
	if( pParts ){
		return dynamic_cast<PartsCounter*>(pParts);
	}

	DEBUG_ASSERT( 0, "指定されたMenuパーツは見つかりませんでした");
	return NULL;
}

bool MenuWindow::DieMain()
{
	if( m_pMainParts->DeletePartsAll() ){
		SAFE_DELETE( m_pMainParts );
		DEBUG_PRINT( "【Window : %s DieMain終了】", m_readMenuFile.c_str() );
		return true;
	}
	return false;
}

bool MenuWindow::Init()
{
	if( !m_pMainParts ){
		m_pMainParts = MenuParts::Create( "root", m_readMenuFile, math::Vector2() );
	}
	return InitMenu();
}

void MenuWindow::Update()
{
	if( m_pMainParts ){
		m_pMainParts->UpdatePartsRecursive();
	}
	UpdateMenu();
}

void MenuWindow::DrawUpdate()
{
	if( m_pMainParts ){
		m_pMainParts->DrawPartsRecursive();
	}
	DrawUpdateMenu();
}

