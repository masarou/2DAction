
#ifndef _MENU_SYS_DEFINE_
#define _MENU_SYS_DEFINE_

//必要なヘッダーインクルード

/* インクルード====================================================================*/
#include "DxLib.h"
#include <string>
#include <vector>
#include <assert.h>
#include <time.h>
#include <fstream>
#include "Common/CmnGameAccesser.h"
/* マクロ==================================================================== */
#define int8_t		char
#define uint8_t		unsigned char
#define int16_t		short
#define uint16_t	unsigned short
#define	int32_t		int
#define	uint32_t	unsigned int
#define	int64_t		long long
#define	uint64_t	unsigned long long
#define NEW new(_NORMAL_BLOCK,__FILE__,__LINE__)
#define SAFE_DELETE(p)      { if (p) { delete (p);     (p)=NULL; } }
#define SAFE_RELEASE(p)		{if(p){(p)->Release();(p)=NULL;}}
#define SAFE_DELETE_ARRAY(p){ if (p) { delete[] (p);   (p)=NULL; } }
#define NUMBEROF(Array)		( sizeof( Array ) / sizeof( Array[ 0 ] ) )
#define INVALID_VALUE UINT_MAX
#define INVALID_FVALUE static_cast<float>(UINT_MAX)

//各データフォルダのパス
static const char *JSON_FLOW_PATH			= "Data/Json/Flow/";
static const char *JSON_GAME2D_PATH			= "Data/Json/Game2D/";
static const char *JSON_SOUND_PATH			= "Data/Json/Sound/";
static const char *BGM_PATH					= "Data/Sound/BGM/";
static const char *SE_PATH					= "Data/Sound/SE/";
static const char *TEXTURE_PATH				= "Data/Texture/";

//定数
#define WINDOW_WIDTH 1366
#define WINDOW_HEIGHT 768

//リークチェック用
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

//データパス
#define TEXTURE "Data/texture/"

#if defined _DEBUG	// デバッグビルド

//アサート
//#define DEBUG_ASSERT( expr, msg ) _ASSERT_EXPR( expr, _T(msg) )
#define DEBUG_ASSERT( expr, msg ) \
	{ \
		if(!expr){ \
			DEBUG_PRINT(msg); \
			assert(expr); \
		} \
	}
#define DEBUG_ASSERTOOO( expr, msg ) _ASSERT_EXPR( expr, msg );

//出力プリント
#define DEBUG_PRINT( str, ... ) \
	{ \
		TCHAR c[256]; \
		sprintf_s( c, str, __VA_ARGS__ ); \
		OutputDebugString( c ); \
	}

#else				// リリースビルド

#define DEBUG_ASSERT(...)
#define DEBUG_PRINT(...)

#endif
#endif