/* ====================================================================== */
/**
 * @brief  Menu���N���X
 *
 * @note
 *		���Json���Ă�ŕ`��݂̂��s��
		pad�̌��m�₻��ɑ΂��鋓�����͔h���N���X�ōs��
 */
/* ====================================================================== */
#ifndef __MENU_WINDOW__
#define __MENU_WINDOW__

#include "Game/Game2DBase.h"
#include "SystemMenuPartsCounter.h"
#include "System/Task/SystemTaskUnit.h"
#include "System/Input/SystemInputWatcher.h"

class MenuParts;

class MenuWindow : public TaskUnit, public InputWatcher
{

public:

protected:

	MenuWindow( const std::string &readMenuJson );
	virtual ~MenuWindow(void);
	
	// �p���֎~!!!
	virtual bool Init() override;
	virtual bool DieMain() override;
	virtual void Update() override;
	virtual void DrawUpdate() override;
	
	// �h�����p
	virtual bool InitMenu(){ return true; }
	virtual void UpdateMenu(){};
	virtual void DrawUpdateMenu(){};

	// �p����eMenu��ʂ���A�j���C�x���g�Z�b�g
	void SetAnim( const std::string &partsStr, const std::string &animStr );

	// �e�p�[�c�N���X�擾
	MenuParts		*GetParts( const std::string &partsStr );
	PartsCounter	*GetPartsCounter( const std::string &partsStr );

private:
	
	std::string	m_readMenuFile;
	MenuParts	*m_pMainParts;

};
#endif // __MENU_WINDOW__