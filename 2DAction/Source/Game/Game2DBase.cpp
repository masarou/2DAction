
/* ====================================================================== */
/**
 * @brief  2D�摜�\��t���N���X
 *
 * @note
 *		��{�I�ɂ͂�����p���A�܂��͕ێ����Ďg�p
 */
/* ====================================================================== */

#include "System/picojson.h"
#include "Game2DBase.h"
#include "System/Draw2D/SystemDraw2DResource.h"

Game2DBase::Game2DBase(const char *jsonFile)
: m_animCounter(0)
, m_jsonFile(jsonFile)
, m_currentAnimTag("")
{
	m_drawInfo.Init();

	//!json������擾
	LoadTextureInfo(m_jsonFile.c_str());
}

Game2DBase::~Game2DBase(void)
{
	DeleteTextureInfo(m_jsonFile.c_str());
}

/* ================================================ */
/**
 * @brief	�ݒ�A�j���̍X�V
 */
/* ================================================ */
void Game2DBase::DrawUpdate2D()
{
	if(m_currentAnimTag.compare("") == 0){
		//!�A�j���������ݒ肳��Ă��Ȃ�
		return ;		
	}

	// �`��̗v�f�ԍ��擾
	uint32_t drawIndex = TextureResourceManager::GetInstance()->GetAnimHandleIndex( m_jsonFile.c_str(), m_currentAnimTag, m_animCounter);

	// �����l���A���Ă����ꍇ�̓A�j���ɂȂɂ��Z�b�g���Ȃ�(�f�t�H���g�A�j���ɂȂɂ��Z�b�g����Ă��Ȃ���)
	if( drawIndex == INVALID_VALUE ){
		m_currentAnimTag = "";
		return;
	}

	// �z��v�f���傫�������ꍇ���������Ȃ�
	if(m_vTexHandle.size() <= drawIndex){
		DEBUG_ASSERT("m_vTexHandle.size() <= drawIndex");
		return;
	}

	// �����擾�ł����Ȃ�`�悷��悤��push���Ă���
	if(drawIndex != INVALID_VALUE){
		Draw2DManager::GetInstance()->PushDrawInfo( m_drawInfo, m_vTexHandle.at(drawIndex));
		++m_animCounter;
	}

}

/* ================================================ */
/**
 * @brief	�Đ��A�j���ύX
 */
/* ================================================ */
void Game2DBase::SetAnim( const std::string &animTag )
{
	if(m_currentAnimTag.compare(animTag.c_str()) == 0){
		//!�Đ����A�j���Ȃ疳��
		return;
	}

	//!�w�肳�ꂽ�A�j�������邩����
	for(uint32_t i = 0; i < m_vAnimName.size(); ++i){
		if(m_vAnimName.at(i).compare(animTag) == 0){
			m_currentAnimTag = animTag;
			m_animCounter = 0;
			return;
		}
	}

	DEBUG_PRINT("�y�w�肳�ꂽ�A�j���^�O������܂���I�z\n");
}

/* ================================================ */
/**
 * @brief	�`��̍ۂɕK�v�ƂȂ���X�V
 */
/* ================================================ */
void Game2DBase::SetDrawInfo( TEX_DRAW_INFO &info)
{
	if( info.m_fileName.compare("") == 0 ){
		info.m_fileName = m_jsonFile;
	}
	m_drawInfo = info;
}

/* ================================================ */
/**
 * @brief	json�t�@�C������A�j�����܂Ƃ߂Ď擾
 */
/* ================================================ */
void Game2DBase::LoadTextureInfo(const char *jsonFile)
{
	// �ǂݍ���
	TextureResourceManager::GetInstance()->LoadTextureInfo(jsonFile);

	// �ǂݍ��񂾃t�@�C��������擾
	TextureResourceManager::GetInstance()->GetTextureHandle( jsonFile, m_vTexHandle);
	TextureResourceManager::GetInstance()->GetPlayAnimName( jsonFile, m_vAnimName);
	m_currentAnimTag = TextureResourceManager::GetInstance()->GetDefaultAnimName( jsonFile );
}

/* ================================================ */
/**
 * @brief	�e�N�X�`���A�����[�h
 */
/* ================================================ */
void Game2DBase::DeleteTextureInfo(const char *jsonFile)
{
	TextureResourceManager::GetInstance()->DeleteTextureInfo(jsonFile);
}