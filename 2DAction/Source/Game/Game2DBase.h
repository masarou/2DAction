/* ====================================================================== */
/**
 * @brief  2D�摜�\��t���N���X
 *
 * @note
 *		��{�I�ɂ͂�����p���A�܂��͕ێ����Ďg�p
 */
/* ====================================================================== */

#ifndef SYSTEM_GAME_2D
#define SYSTEM_GAME_2D

#include "System/SystemDefine.h"
#include "System/Task/SystemTaskUnit.h"
#include "System/Draw2D/SystemDraw2DDefine.h"
#include "System/Draw2D/SystemDraw2DManager.h"

class Game2DBase
{
public:

	Game2DBase( const char *jsonFile );
	virtual ~Game2DBase(void);

	//!�Đ����A�j���̕ύX�A�擾
	void SetAnim( const std::string &animTag );							
	const char *GetPlayAnim(){return m_currentAnimTag.c_str();}
	
	//!�`��ݒ�X�V
	void SetDrawInfo( TEX_DRAW_INFO &info );
	
	//!�ݒ�A�j���̍X�V
	void DrawUpdate2D();

	const TEX_DRAW_INFO &GetDrawInfo(){ return m_drawInfo; }

protected:



private:

	//!�e�N�X�`�����[�h
	void LoadTextureInfo(const char *jsonFile);
	//!�e�N�X�`���A�����[�h
	void DeleteTextureInfo(const char *jsonFile);

	// �f�t�H���g�A�j���Ajson�t�@�C�����A�A�j���̎�ސ�

	uint32_t						m_animCounter;		//!<�A�j���̍ۂɎg�p����J�E���^
	std::string						m_jsonFile;			// �ǂݍ���json�t�@�C����
	TEX_DRAW_INFO					m_drawInfo;

	std::string						m_currentAnimTag;	//!<�Đ����̃A�j��
	std::vector<int32_t>			m_vTexHandle;		// �`��Handle
	std::vector<std::string>		m_vAnimName;		// �Đ��ł���A�j��
	
};

#endif