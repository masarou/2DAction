/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		FPS�Ǘ��N���X
 */
/* ====================================================================== */

#ifndef SYSTEM_FPS_MANAGER
#define SYSTEM_FADE_MANAGER

#include "SystemDefine.h"

class FpsManager
{
public:
	static FpsManager *Create();
	static FpsManager *GetInstance();

	static void Destroy();

	//! �t���[���X�V�^�C�~���O���`�F�b�N
	bool IsUpdateTiming();

	//! FPS�̐ݒ�
	static void SetFps( uint32_t fps );
	//! FPS�̎擾
	static const uint32_t &GetFps();
	static float GetFpsF();

private:
	FpsManager();
	~FpsManager();

	static FpsManager *s_pInstance;
	static uint32_t m_fps;
};

#endif
