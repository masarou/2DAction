
/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		FPS�Ǘ��N���X
 */
/* ====================================================================== */


#include "SystemFPSManager.h"

FpsManager *FpsManager::s_pInstance = NULL;
uint32_t FpsManager::m_fps = 60;
uint32_t FpsManager::m_updateCounter = 0;

// �����v�Z�p
namespace{
static uint32_t s_instancCreateTime = 0;	// �C���X�^���X�������̎���
static uint32_t s_preUpdateTime = 0;		// �O�̃t���[���̍X�V����
}

/* ================================================ */
/**
 * @brief	�R���X�g���N�^
 */
/* ================================================ */
FpsManager::FpsManager()
{
}

/* ================================================ */
/**
 * @brief	�f�X�g���N�^
 */
/* ================================================ */
FpsManager::~FpsManager()
{
	s_pInstance = NULL;
}

/* ================================================ */
/**
 * @brief	����
 */
/* ================================================ */
FpsManager* FpsManager::Create()
{
	if( s_pInstance == NULL ){
		s_pInstance = NEW FpsManager;

		s_instancCreateTime = GetNowCount();	// �A�v���J�n���̎���
		s_preUpdateTime = s_instancCreateTime;
	}

	return s_pInstance;
}

/* ================================================ */
/**
 * @brief	�C���X�^���X�擾
 */
/* ================================================ */
FpsManager* FpsManager::GetInstance()
{
	return s_pInstance;
}

/* ================================================ */
/**
 * @brief	�j��
 */
/* ================================================ */
void FpsManager::Destroy()
{
	SAFE_DELETE( s_pInstance );
}

/* ================================================ */
/**
 * @brief	�t���[���X�V�^�C�~���O���`�F�b�N
 */
/* ================================================ */
bool FpsManager::IsUpdateTiming()
{
	uint32_t nowTime = GetNowCount();

	//! �C���X�^���X�쐬����̃t���[��
	uint32_t nowFrame = (nowTime - s_instancCreateTime)*m_fps/1000;
	//! �O��X�V���̃t���[��
	uint32_t framePreUpdate = (s_preUpdateTime - s_instancCreateTime)*m_fps/1000;

	if( nowFrame != framePreUpdate ){
		//! �X�V���Ԃ��X�V
		s_preUpdateTime = nowTime;
		++m_updateCounter;
		return true;
	}else{
		//! �܂��t���[���X�V�^�C�~���O�łȂ�
		return false;
	}
}

/* ================================================ */
/**
 * @brief	FPS�̐ݒ�
 */
/* ================================================ */
void FpsManager::SetFps( uint32_t fps )
{
	m_fps = fps;
}

/* ================================================ */
/**
 * @brief	FPS�̎擾
 */
/* ================================================ */
const uint32_t &FpsManager::GetFps()
{
	return m_fps;
}

/* ================================================ */
/**
 * @brief	FPS�̎擾 float
 */
/* ================================================ */
float FpsManager::GetFpsF()
{
	return static_cast<float>(m_fps);
}

/* ================================================ */
/**
 * @brief	�Q�[���̍X�V�J�E���^�擾
 */
/* ================================================ */
const uint32_t FpsManager::GetUpdateCounter()
{
	return m_updateCounter;
}
