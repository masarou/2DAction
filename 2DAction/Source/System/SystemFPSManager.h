/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		FPS�Ǘ��N���X
 */
/* ====================================================================== */

#ifndef __SYSTEM_FPS_MANAGER__
#define __SYSTEM_FADE_MANAGER__

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
	//! �X�V�񐔎擾
	static const uint32_t GetUpdateCounter();

private:
	FpsManager();
	~FpsManager();

	static FpsManager *s_pInstance;
	static uint32_t m_fps;
	static uint32_t m_updateCounter;	//! �Q�[�����J�E���g�A�b�v(60fps)
};

#endif
