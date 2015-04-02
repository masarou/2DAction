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
#include "System/Task/SystemTaskUnit.h"
#include "System/Collision/SystemCollisionUnit.h"

class PlayerStatusMenu : public TaskUnit
{

public:

	PlayerStatusMenu();
	~PlayerStatusMenu(void);

	void AddItemLevel( const Common::ITEM_KIND $kind );
	const uint32_t GetItemLevel( const Common::ITEM_KIND &kind ) const;

	void SetPlayerHP( const uint32_t &val, const uint32_t &lifeMax );

protected:

	virtual bool Init() override;
	virtual bool DieMain() override;
	virtual void Update() override;			// �ړ����̓������l�̍X�V
	virtual void DrawUpdate() override;		// �`��X�V

private:

	// �w��p�[�c�̏��擾
	const math::Vector2 GetPartsPos( const std::string name ) const;
	const Common::PARTS_INFO &GetPartsInfo( const std::string name ) const;

	// �`��
	Texture2D		m_textureStatus;		// �X�e�[�^�X�摜
	Texture2D		m_lifeGauge;			// ���C�t�Q�[�W
	Texture2D		m_danageLevel;			// �U���̓��x������
	Texture2D		m_rapidLevel;			// �A�ˑ��x���x������

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
