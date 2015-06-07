/* ====================================================================== */
/**
 * @brief  ��ʂ��\������p�[�c�ЂƂ̃N���X
 *
 * @note
 *		
 */
/* ====================================================================== */
#ifndef __SYSTEM_MENU_PARTS__
#define __SYSTEM_MENU_PARTS__

#include "System/SystemDefine.h"
#include "Common/CommonDefine.h"
#include "Game/Game2DBase.h"

class MenuParts
{

public:

	static MenuParts *Create( const std::string &partsStr, const std::string &jsonStr, const math::Vector2 &originalPos );

	~MenuParts(void);

	// �I���֐�
	bool DeletePartsAll();

	// �A�j����ς������Ƃ��Ɏg�p
	bool SetPartsAnim( const std::string &partsName, const std::string &animStr );

	// �`����擾���X�V
	TEX_DRAW_INFO &GetTexDrawInfo(){ return m_texMine.m_texInfo; }

	// �q���܂ߕ`��g���K�[�֐�
	void DrawParts();

	// �e�h����ŕK�v�ɂȂ�Update
	virtual void UpdateParts(){};

	// �p�[�c�^�C�v�擾(�h����ł͕K������)
	virtual Common::PARTS_TYPE_MENU GetPartsType(){ return Common::PARTS_SINGLE_DRAW; }

	// �w�肳�ꂽ�p�[�c�N���X���ċA�I�ɒT���Ȃ����NULL
	MenuParts *GetPartsRecursive( const std::string &partsStr );

protected:

	MenuParts( const std::string &partsStr, const std::string &jsonStr, const math::Vector2 &originalPos );

private:

	// �p�[�c�̃Z�b�g�A�b�v�ċA�֐�
	void SetupParts();

protected:
	
	std::string									m_partsNameStr;	// �e�ɖ��Â���ꂽ�p�[�c��
	std::string									m_readJsonStr;	// �ǂݍ���json�t�@�C����
	math::Vector2								m_originPos;	// �e�N���X�Ɏw�肳�ꂽ�ʒu���

	Texture2D									m_texMine;		// ���g�̃e�N�X�`���N���X

	std::vector<MenuParts*>						m_partsArray;	// ���̃N���X�̃p�[�c�ɕt�����Ă���p�[�c
	std::map< std::string, Common::PARTS_INFO >	m_partsMap;		// �t���p�[�c�̏��
};

#endif // __SYSTEM_MENU_PARTS__