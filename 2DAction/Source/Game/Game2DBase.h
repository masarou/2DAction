/* ====================================================================== */
/**
 * @brief  2D�摜�\��t���N���X
 *
 * @note
 *		��{�I�ɂ͂�����p���A�܂��͕ێ����Ďg�p
 */
/* ====================================================================== */

#ifndef __SYSTEM_GAME_2D__
#define __SYSTEM_GAME_2D__

#include "System/SystemDefine.h"
#include "System/Task/SystemTaskUnit.h"
#include "System/Draw2D/SystemDraw2DDefine.h"
#include "System/Draw2D/SystemDraw2DManager.h"

class Game2DBase
{
public:

	// CreateWithCheck�͎w�肳�ꂽJson��Game2D�p�̂��̂��ǂ����̃`�F�b�N���s��
	static Game2DBase *Create( const char *jsonFile );
	static Game2DBase *CreateWithCheck( const char *jsonFile );
	
	virtual ~Game2DBase(void);

	//!�Đ����A�j���̕ύX�A�擾
	void SetAnim( const std::string &animTag );
	const char *GetPlayAnim() const{return m_currentAnimTag.c_str();}
	
	//!�`��ݒ�X�V
	void SetDrawInfo( TEX_DRAW_INFO &info );
	
	//!�ݒ�A�j���̍X�V
	void DrawUpdate2D();

	//!���݂̃A�j���Đ��t���[�����擾
	const uint32_t &GetCurrentAnimFrame() const{ return m_animCounter; }

	const TEX_DRAW_INFO &GetDrawInfo() const{ return m_drawInfo; }
	TEX_DRAW_INFO &UpdateDrawInfo(){ return m_drawInfo; }
	
private:

	Game2DBase( const char *jsonFile );

	//!�e�N�X�`�����[�h
	void LoadTextureInfo(const char *jsonFile);
	//!�e�N�X�`���A�����[�h
	void DeleteTextureInfo(const char *jsonFile);

	// �f�t�H���g�A�j���Ajson�t�@�C�����A�A�j���̎�ސ�

	uint32_t						m_animCounter;		//!<�A�j���̍ۂɎg�p����J�E���^
	TEX_DRAW_INFO					m_drawInfo;

	std::string						m_currentAnimTag;	//!<�Đ����̃A�j��
	std::vector<int32_t>			m_vTexHandle;		// �`��Handle
	std::vector<std::string>		m_vAnimName;		// �Đ��ł���A�j��

};

struct Texture2D{
	Game2DBase			*m_pTex2D;			// �摜�Ǘ�
	//TEX_DRAW_INFO		m_texInfo;			// �`����

	Texture2D(){
		Init();
	}

	void Init(){
		m_pTex2D = NULL;
		//m_texInfo.Init();
	}
	void DeleteAndInit(){
		SAFE_DELETE(m_pTex2D);
		//m_texInfo.Init();
	}
};

#endif