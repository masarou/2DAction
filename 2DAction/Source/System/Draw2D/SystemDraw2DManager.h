/* ====================================================================== */
/**
 * @brief  ‚Q‚c•`‰æ‚ğ‚Ü‚Æ‚ß‚Äs‚¤ƒNƒ‰ƒX
 *
 * @note
 *		ƒvƒŒƒCƒ„[A“G‚È‚Ç‚Ì•`‰æŠG‚Í‚·‚×‚Ä‚±‚±‚Å•`‰æ—\–ñ‚³‚ê‚½Œã‚Ü‚Æ‚ß‚Ä•`‰æ‚·‚é
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

	static void Create();
	static Draw2DManager *GetInstance();

	//! ‡˜‚ğŒˆ‚ß‚Ä•`‰æ
	void Action();

	//! •`‰æ—\–ñ
	void PushDrawInfo( const TEX_DRAW_INFO &texInfo, const int32_t &handle, const KIND_2D &kind = KIND_NORMAL, const PRIORITY &priority = PRIORITY_NORMAL );

	//! •`‰æ—\–ñƒLƒƒƒ“ƒZƒ‹
	void DeleteDrawInfo( const int32_t &handle );
	void DeleteDrawInfo( const char *jsonFile );
	
	//! •`‰æ‚ğŠŠ‚ç‚©‚É‚·‚é‚©ƒtƒ‰ƒO
	void SetUseBilinear( const bool &flag ){m_useBilinear = flag;}

	//! ƒNƒ‰ƒX‚Ì‰ğ•ú
	void DeleteDraw2DManager();

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

	Draw2DManager(void);
	~Draw2DManager(void);

	//! •`‰æw¦
	void DrawTexture( const uint32_t &drawIndex);

	static Draw2DManager	*s_pInstance;

	bool					m_useBilinear;
	std::vector<DRAW2D>		m_vDrawTask;
};

#endif //SYSTEM_DRAW2D_MASNAGER