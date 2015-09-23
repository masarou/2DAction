/* ====================================================================== */
/**
 * @brief  �v���C���[�̃X�e�[�^�X�ꊇ�\�����
 *
 * @note
 *		
 */
/* ====================================================================== */

#ifndef __GAME_PLAYER_STATUS_MENU__
#define __GAME_PLAYER_STATUS_MENU__

#include "Common/CommonDefine.h"
#include "Game/Game2DBase.h"
#include "Game/Item/ItemObject.h"
#include "System/Menu/SystemMenuWindow.h"

class PlayerStatusMenu : public MenuWindow
{

public:

	PlayerStatusMenu();
	~PlayerStatusMenu(void);

	void AddItemLevel( const Common::ITEM_KIND $kind );
	const uint32_t GetItemLevel( const Common::ITEM_KIND &kind ) const;

	void SetPlayerHP( const uint32_t &val, const uint32_t &lifeMax );

protected:
	
	virtual bool InitMenu() override;
	virtual void UpdateMenu() override;
	virtual void DrawUpdateMenu() override;
	virtual bool DieMainMenu() override;

private:

	// �`��
	Texture2D		m_textureStatus;		// �X�e�[�^�X�摜
	Texture2D		m_lifeGauge;			// ���C�t�Q�[�W

	// �v���C���[���C�t
	uint32_t		m_playerLife;
	uint32_t		m_playerLifeMax;

	// �A�C�e�����x��
	uint32_t		m_itemLevelDamage;		// �З̓��x��
	uint32_t		m_itemLevelRapid;		// �A�˃��x��

	// �p�[�c�}�b�v
	std::map< std::string, Common::PARTS_INFO >	m_partsMap;
};

#endif
