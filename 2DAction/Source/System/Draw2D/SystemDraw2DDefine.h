/* ====================================================================== */
/**
 * @brief  ２Ｄ描画DEFINE
 *
 * @note
 *		
 */
/* ====================================================================== */

#ifndef SYSTEM_DRAW2D_DEFINE
#define SYSTEM_DRAW2D_DEFINE

#include "Math/MathCommon.h"
#include "Math/MathVector.h"
#include "Math/MathAngle.h"
#include "System/SystemDefine.h"

#define SPLIT_MAX 80

//描画の種類 優先
enum KIND_2D{
	KIND_BACK,
	KIND_NORMAL,
	KIND_FILTER,

	KIND_MAX,
};

enum PRIORITY{
	PRIORITY_LOWEST,
	PRIORITY_LOW,
	PRIORITY_BELOW_NORMAL,	// 標準以下
	PRIORITY_NORMAL,		// 標準
	PRIORITY_ABOVE_NORMAL,	// 標準以上
	PRIORITY_HIGH,
	PRIORITY_HIGHEST,

	PRIORITY_MAX,
};

//!アニメーションの情報構造体
struct ANIM_INFO{
	std::string				m_animTag;		// アニメ識別子
	std::string				m_nextAnimTag;	// 再生終了時に次に再生したいアニメがあれば設定
	std::vector<uint32_t>	m_vPlayIndex;	// アニメの再生絵の番号
	uint32_t				m_frameSpeed;	// 再生スピード
	void Init(){
		m_animTag = "";
		m_nextAnimTag = "";
		m_vPlayIndex.clear();
		m_frameSpeed = 10;
	}
};

//テクスチャを読み込むときの情報
struct TEX_INIT_INFO{
	std::string m_fileName;
	uint32_t	m_splitNum;
	uint32_t	m_splitWidth;	//横何枚か
	uint32_t	m_splitHeight;	//縦何枚か
	uint32_t	m_sizeWidth;	//横サイズ
	uint32_t	m_sizeHeight;	//縦サイズ
	std::string m_animDefault;	//デフォルトアニメ
	std::vector<ANIM_INFO> m_vAnimName;	//アニメ情報
	void Init(){
		m_fileName		= "";
		m_splitNum		= 0;
		m_splitWidth	= 0;
		m_splitHeight	= 0;
		m_sizeWidth		= 0;
		m_sizeHeight	= 0;
		m_animDefault	= "";
		m_vAnimName.clear();
	}
};

//描画更新の際の情報
struct TEX_DRAW_INFO{
	std::string		m_fileName;
	math::Vector2	m_scale;			//!<拡大率
	math::Vector2	m_pos;				//!<位置情報
	math::Vector2	m_arrangeOrigin;	//!<拡大、回転時の原点
	bool			m_usePlayerOffset;	//!<位置補正の使用有無
	math::Angle		m_rot;				//!<回転情報
	uint32_t		m_alpha;			//!<透過情報
	KIND_2D			m_kind2D;			//!<2Dの種類
	PRIORITY		m_prioity;			//!<描画優先度
	void Init(){
		m_fileName = "";
		m_scale = math::Vector2( 1.0f, 1.0f );
		m_pos = math::Vector2();
		m_arrangeOrigin = math::Vector2( INVALID_FVALUE, INVALID_FVALUE );
		m_usePlayerOffset = true;
		m_rot = math::Angle();
		m_alpha = 255;
		m_kind2D = KIND_NORMAL;
		m_prioity = PRIORITY_NORMAL;
	}
};


#endif