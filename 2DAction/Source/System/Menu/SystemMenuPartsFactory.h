/* ====================================================================== */
/**
 * @brief  メニューパーツのファクトリ関数
 *
 * @note
 *		
 */
/* ====================================================================== */
#ifndef __SYSTEM_MENU_PARTS_FACTORY__
#define __SYSTEM_MENU_PARTS_FACTORY__

#include "Common/CommonDefine.h"
#include "System/Menu/SystemMenuParts.h"
#include "System/Menu/SystemMenuPartsCounter.h"

typedef MenuParts *(*pFunkRetMenuParts)( const std::string &partsStr, const std::string &jsonStr, const Common::PRIORITY &priority, const math::Vector2 &originalPos );

struct PARTS_MAP{
	pFunkRetMenuParts			pCallFunction;
	Common::PARTS_TYPE_MENU		typeKey;
};

static const PARTS_MAP s_partsTable[] = {
	{ MenuParts::Create,		Common::PARTS_SINGLE_DRAW },
	{ PartsCounter::Create,		Common::PARTS_NUM_COUNTER },
};

static MenuParts *CreatePartsFactory( const Common::PARTS_TYPE_MENU &type, const std::string &partsStr, const std::string &jsonStr, const Common::PRIORITY &priority, const math::Vector2 &originalPos = math::Vector2() )
{
	for( uint32_t i = 0; i < NUMBEROF(s_partsTable); ++i ){
		if( type == s_partsTable[i].typeKey ){
			return (*s_partsTable[i].pCallFunction)( partsStr, jsonStr, priority, originalPos );
		}
	}

	DEBUG_ASSERT( 0, "想定外のパーツタイプが来た");
	return NULL;
}
#endif
