/* ====================================================================== */
/**
 * @brief  ２Ｄ描画をまとめて行うクラス
 *
 * @note
 *		プレイヤー、敵などの描画絵はすべてここで描画予約された後まとめて描画する
 */
/* ====================================================================== */

#ifndef __SYSTEM_DRAW2D_MASNAGER__
#define __SYSTEM_DRAW2D_MASNAGER__

#include "Math/MathCommon.h"
#include "Math/MathVector.h"
#include "Math/MathAngle.h"
#include "System/SystemDefine.h"
#include "System/Draw2D/SystemDraw2DDefine.h"

class Draw2DManager
{
public:

	static void Create();
	static Draw2DManager *GetInstance();

	//! 順序を決めて描画
	void Action();

	//! 描画予約
	void PushDrawString( const std::string &str, const math::Vector2 &pos, const uint32_t &color = GetColor( 0, 0, 0 ) );
	void PushDrawInfo( const TEX_DRAW_INFO &texInfo, const int32_t &handle, const Common::PRIORITY &priority = Common::PRIORITY_NORMAL );

	//! 描画予約キャンセル
	void DeleteDrawInfo( const int32_t &handle );
	void DeleteDrawInfo( const char *jsonFile );
	
	//! 描画を滑らかにするかフラグ
	void SetUseBilinear( const bool &flag ){m_useBilinear = flag;}

	//! クラスの解放
	void DeleteDraw2DManager();

#ifdef _DEBUG
	struct DrawCircleInfo{
		float radius;
		math::Vector2 pos;
	};
	std::vector<DrawCircleInfo> m_drawCircle;
	struct DrawBoxInfo{
		math::Vector2 leftUpper;
		uint32_t sideDistance;
	};
	std::vector<DrawBoxInfo> m_drawBox;
#endif

private:

	struct DRAW2D{
		int32_t	m_handle;
		TEX_DRAW_INFO m_info;
		uint8_t loadCounter;
		void Init(){
			m_info.Init();
			m_handle	= INVALID_VALUE;
			loadCounter = 0;
		}
	};

	struct DRAWSTR{
		std::string		m_str;
		math::Vector2	m_drawPos;
		uint32_t		m_color;
		void Init(){
			m_str		= "";
			m_drawPos	= math::Vector2();
			m_color		= GetColor( 255,255,255 );
		}
	};

	Draw2DManager(void);
	~Draw2DManager(void);

	//! 文字列を成形して描画(\nがあれば改行を行う)
	void DrawString( const DRAWSTR &drawTask );

	//! 描画指示
	void DrawTexture( const uint32_t &drawIndex );
	void PushDrawInfoMain( const TEX_DRAW_INFO &texInfo, const int32_t &handle, const Common::PRIORITY &priority = Common::PRIORITY_NORMAL );

	static Draw2DManager	*s_pInstance;

	bool					m_useBilinear;
	std::vector<DRAW2D>		m_vDrawTask;
	std::vector<DRAWSTR>	m_vDrawStringTask;
};

#endif //SYSTEM_DRAW2D_MASNAGER