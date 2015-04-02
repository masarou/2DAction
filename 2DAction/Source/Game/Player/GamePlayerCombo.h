/* ====================================================================== */
/**
 * @brief  �v���C���[�̃R���{�\�����s��
 *
 * @note
 *		
 */
/* ====================================================================== */

#ifndef __GAME_PLAYER_COMBO__
#define __GAME_PLAYER_COMBO__

#include "Common/CommonDefine.h"
#include "Common/CmnNumberCounter.h"
#include "Game/Game2DBase.h"
#include "Game/Item/ItemObject.h"
#include "System/Task/SystemTaskUnit.h"
#include "System/Collision/SystemCollisionUnit.h"

class PlayerCombo : public TaskUnit
{

public:

	static PlayerCombo *CreatePlayerCombo();
	~PlayerCombo(void);

	void AddItemLevel( const Common::ITEM_KIND $kind );
	const uint32_t GetItemLevel( const Common::ITEM_KIND &kind ) const;

	void SetPlayerHP( const uint32_t &val, const uint32_t &lifeMax );

protected:

	virtual bool Init() override;
	virtual bool DieMain() override;
	virtual void Update() override;			// �ړ����̓������l�̍X�V
	virtual void DrawUpdate() override;		// �`��X�V

private:
	
	PlayerCombo();
	// �p�[�c���Z�b�g
	void SetupPartsInfo();
	// �w��p�[�c�̏��擾
	const math::Vector2 GetPartsPos( const std::string name ) const;
	const Common::PARTS_INFO &GetPartsInfo( const std::string name ) const;


	// �`��
	Texture2D		m_textureFrame;				// �R���{��\������O�g
	Texture2D		m_textureCombo;				// �u�R���{�v������
	Texture2D		m_textureComboGauge;		// �R���{�Q�[�W
	NumberCounter	*m_pNumCounterCombo;		// �R���{��(�ʏ�)
	NumberCounter	*m_pNumCounterComboYellow;	// �R���{��(��)
	NumberCounter	*m_pNumCounterComboRed;		// �R���{��(��)

	// �p�[�c�}�b�v
	std::map< std::string, Common::PARTS_INFO >	m_partsMap;
};

#endif
