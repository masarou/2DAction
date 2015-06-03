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

	MenuParts( const std::string &partsStr, const std::string &jsonStr, const math::Vector2 &originalPos );
	~MenuParts(void);

	// �q�p�[�c�N���X�܂ߕ`��g���K�[�֐�
	void DrawParts();

	// �I���֐�
	bool DeletePartsAll();

	// �A�j����ς������Ƃ��Ɏg�p
	bool SetPartsAnim( const std::string &partsName, const std::string &animStr );

protected:


private:

	// �p�[�c�̃Z�b�g�A�b�v�ċA�֐�
	void SetupParts();
	
	std::string									m_partsNameStr;	// �e�ɖ��Â���ꂽ�p�[�c��
	std::string									m_readJsonStr;	// �ǂݍ���json�t�@�C����
	math::Vector2								m_originPos;	// �e�N���X�Ɏw�肳�ꂽ�ʒu���

	Texture2D									m_texMine;		// ���g�̃e�N�X�`���N���X

	std::vector<MenuParts*>						m_partsArray;	// ���̃N���X�̃p�[�c�ɕt�����Ă���p�[�c
	std::map< std::string, Common::PARTS_INFO >	m_partsMap;		// �t���p�[�c�̏��
};
#endif // __SYSTEM_MENU_PARTS__