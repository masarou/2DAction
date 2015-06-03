/* ====================================================================== */
/**
 * @brief  ”wŒiŠÇ—ƒNƒ‰ƒX
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

bool MenuWindow::DieMain()
{
	if( m_pMainParts->DeletePartsAll() ){
		SAFE_DELETE( m_pMainParts );
		DEBUG_PRINT( "yWindow : %s DieMainI—¹z", m_readMenuFile.c_str() );
		return true;
	}
	return false;
}

bool MenuWindow::Init()
{
	if( !m_pMainParts ){
		m_pMainParts = NEW MenuParts( "root", m_readMenuFile, math::Vector2() );
	}
	return InitMenu();
}

void MenuWindow::DrawUpdate()
{
	if( m_pMainParts ){
		m_pMainParts->DrawParts();
	}
}

