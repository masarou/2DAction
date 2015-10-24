
#ifndef __MENU_SYS_DEFINE__
#define __MENU_SYS_DEFINE__

//必要なヘッダーインクルード

/* インクルード====================================================================*/
#include "DxLib.h"
#include <string>
#include <vector>
#include <map>
#include <assert.h>
#include <time.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include "yield.hpp"
#include "Common/CmnGameAccesser.h"
#include "Common/ConvertDxFuncToIfstream.h"

/* 定数==================================================================== */
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define FONT_DEFAULT_SIZE 25

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
#define DEFAULT_VECTOR2	math::Vector2()
#define DEFAULT_VECTOR3	math::Vector3()
#define WINDOW_CENTER	math::Vector2( static_cast<float>(WINDOW_WIDTH / 2), static_cast<float>(WINDOW_HEIGHT / 2 ) )

//各データフォルダのパス
static const char *JSON_FLOW_PATH			= "Data/Json/Flow/";
static const char *JSON_GAME2D_PATH			= "Data/Json/Game2D/";
static const char *JSON_GAME2D_MAP_PATH		= "Data/Json/Game2D/Map/";
static const char *JSON_SOUND_PATH			= "Data/Json/Sound/";
static const char *JSON_OTHER_PATH			= "Data/Json/Other/";
static const char *BGM_PATH					= "Data/Sound/BGM/";
static const char *SE_PATH					= "Data/Sound/SE/";
static const char *TEXTURE_PATH				= "Data/Texture/";

//リークチェック用
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#if defined _DEBUG	// デバッグビルド

//アサート
//#define DEBUG_ASSERT( expr, msg ) _ASSERT_EXPR( expr, _T(msg) )
#define DEBUG_ASSERT( expr, msg ) \
	(void) ((!!(expr)) || \
	(1 != _CrtDbgReport(_CRT_ASSERT, __FILE__, __LINE__, NULL, msg )) || \
	(_CrtDbgBreak(), 0))

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