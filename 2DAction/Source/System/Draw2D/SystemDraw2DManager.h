/* ====================================================================== */
/**
 * @brief  ２Ｄ描画をまとめて行うクラス
 *
 * @note
 *		プレイヤー、敵などの描画絵はすべてここで描画予約された後まとめて描画する
 */
/* ====================================================================== */

#ifndef SYSTEM_DRAW2D_MASNAGER
#define SYSTEM_DRAW2D_MASNAGER

#include "Math/MathCommon.h"
#include "Math/MathVector.h"
#include "Math/MathAngle.h"
#include "System/SystemDefine.h"
#include "System/Draw2D/SystemDraw2DDefine.h"

class Draw2DManager
{
public:

	Draw2DManager(void);
	~Draw2DManager(void);

	static void Create();
	static Draw2DManager *GetInstance();

	//! 順序を決めて描画
	void Action();

	//! 描画予約
	void PushDrawInfo( const TEX_DRAW_INFO &texInfo, const int32_t &handle, const KIND_2D &kind = KIND_NORMAL, const PRIORITY &priority = PRIORITY_NORMAL );

	//! 描画予約キャンセル
	void DeleteDrawInfo( const int32_t &handle );
	
	//! 描画を滑らかにするかフラグ
	void SetUseBilinear( const bool &flag ){m_useBilinear = flag;}

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

	//! 描画指示
	void DrawTexture( const uint32_t &drawIndex);

	static Draw2DManager	*m_pInstance;

	bool					m_useBilinear;
	std::vector<DRAW2D>		m_vDrawTask;
};

#endif //SYSTEM_DRAW2D_MASNAGER