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
#include "System/Draw2D/SystemDraw2DDefine.h"

class MenuParts;

class MenuWindow : public TaskUnit, public InputWatcher
{

public:

	const uint32_t &GetSelectedNo() const{ return m_selectNo; }
	const std::string GetNextFlowStr() const{ return m_nextFlow; }

protected:

	MenuWindow( const std::string &readMenuJson, const Common::PRIORITY &priority = Common::PRIORITY_NORMAL, const math::Vector2 &partsPos = math::Vector2() );
	virtual ~MenuWindow(void);
	
	// �p���֎~!!!
	virtual bool Init() override;
	virtual bool DieMain() override;
	virtual void Update() override;
	virtual void DrawUpdate() override;
	
	// �h�����p
	virtual bool InitMenu(){ return true; }
	virtual bool DieMainMenu(){ return true; }
	virtual void UpdateMenu(){};
	virtual void DrawUpdateMenu(){};

	// �p����eMenu��ʂ���A�j���C�x���g�Z�b�g
	void SetAnim( const std::string &partsStr, const std::string &animStr );
	void SetAnim( const std::string &partsStr, const uint32_t &num );
	void SetSelectNum( const uint32_t &selNum ){ m_selectNo = selNum; }
	void SetNextFlowStr( const std::string &nextFlowStr ){ m_nextFlow = nextFlowStr; }

	// �e�p�[�c�N���X�擾
	MenuParts		*GetParts( const std::string &partsStr );
	PartsCounter	*GetPartsCounter( const std::string &partsStr );

private:
	
	std::string	m_readMenuFile;
	MenuParts	*m_pMainParts;
	
	math::Vector2		m_posMainParts;	// ��ʑS�̂̈ʒu���
	Common::PRIORITY	m_priority;		// �`��D�揇��
	uint32_t			m_selectNo;		// �I�����ԍ�(��ʂɂ���Ă͎g�p���Ȃ����Ƃ�����)
	std::string			m_nextFlow;		// ���̑J�ڐ�

};
#endif // __MENU_WINDOW__