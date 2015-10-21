/* ====================================================================== */
/**
 * @brief  
 *		�Q�[���J�n�O�A���[�U�[�̊�{�\��UP���
 * @note
 *		
 */
/* ====================================================================== */
#ifndef __FLOW_POWER_UP__
#define __FLOW_POWER_UP__

#include "FlowMenuBase.h"
#include "Game/Game2DBase.h"
#include "System/Task/SystemTaskUnit.h"
#include "System/Menu/SystemMenuWindow.h"

class PowerUpMenu;

class FlowPowerUpPlayer : public FlowMenuBase
{
public:

	static FlowBase *Create( const std::string &fileName );

protected:
	
	virtual bool Init() override;

private:

	FlowPowerUpPlayer( const std::string &fileName );
	~FlowPowerUpPlayer(void);

};

/* ====================================================================== */
/**
 * @brief  
 *		FlowPowerUpPlayer�̕\�������g
 * @note
 *		
 */
/* ====================================================================== */
class PowerUpMenu : public MenuWindow
{
public:

	enum{
		SEELCT_ITEM,
		SELECT_GAME_START,

		SELECT_MAX,
	};

	static PowerUpMenu *CreatePowerUp2D( const std::string &fileName );

protected:
	
	virtual bool InitMenu() override;
	virtual bool DieMainMenu() override;
	virtual void UpdateMenu() override;

	virtual void PadEventDecide() override;
	virtual void PadEventUp() override;
	virtual void PadEventDown() override;
	virtual void PadEventRight() override;
	virtual void PadEventLeft() override;
	virtual void PadEventCommand( const uint32_t &commandKind ) override;

private:

	PowerUpMenu( const std::string &fileName );
	virtual ~PowerUpMenu(void);

	// �`�悷�鋭�����ڐ����X�V
	void ChangeDispState( const Common::PLAYER_BASE_STATE &kind );

	// �J�[�\���ړ��̏����܂Ƃ�
	void UpdateCursorMove();

	// ���ڂ��Ƃ̐������擾
	std::string GetExplanationStr( const Common::PLAYER_BASE_STATE &kind );

	// ���̃��x���܂ł̃|�C���g���擾
	uint32_t GetPointToNextLevel( const Common::PLAYER_BASE_STATE &kind, uint32_t currLevel );

	bool						m_isProceedEnd;	// ����i�s�ŉ�ʂ��I���������ǂ���

	Common::SAVE_DATA			m_loadData;
	Common::PLAYER_BASE_STATE	m_selectStateKind;

	std::string					m_explanationStr;
};

#endif