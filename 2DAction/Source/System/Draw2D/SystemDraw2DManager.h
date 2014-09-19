/* ====================================================================== */
/**
 * @brief  �Q�c�`����܂Ƃ߂čs���N���X
 *
 * @note
 *		�v���C���[�A�G�Ȃǂ̕`��G�͂��ׂĂ����ŕ`��\�񂳂ꂽ��܂Ƃ߂ĕ`�悷��
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

	//! ���������߂ĕ`��
	void Action();

	//! �`��\��
	void PushDrawInfo(TEX_DRAW_INFO &texInfo, int32_t &handle, KIND_2D kind = KIND_NORMAL, PRIORITY priority = PRIORITY_MIDDLE);

	//! �`��\��L�����Z��
	void DeleteDrawInfo(int32_t &handle);
	
	//! �`������炩�ɂ��邩�t���O
	void SetUseBilinear(bool flag){m_useBilinear = flag;}

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

	//! �`��w��
	void DrawTexture(uint32_t drawIndex);

	static Draw2DManager	*m_pInstance;

	bool					m_useBilinear;
	std::vector<DRAW2D>		m_vDrawTask;
};

#endif //SYSTEM_DRAW2D_MASNAGER