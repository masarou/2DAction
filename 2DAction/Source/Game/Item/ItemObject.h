/* ====================================================================== */
/**
 * @brief  �t�B�[���h�ɖ���ׂɏo�Ă���A�C�e���N���X
 *
 * @note
 *		
 */
/* ====================================================================== */

#ifndef ITEM_OBJECT
#define ITEM_OBJECT

#include "Game/Game2DBase.h"
#include "System/Task/SystemTaskUnit.h"

class ItemObject : public TaskUnit
{
public:

	// �Œ�l
	static const uint32_t ITEM_LIVE_TIME = 900;	// �A�C�e�����ݎ���

	enum ITEM_KIND{
		ITEM_KIND_RAPID_BULLET,	// �A�ˑ��xUP
		ITEM_KIND_LIFE_UP,		// ���C�t��
		ITEM_KIND_DAMAGE_UP,	// �e�̈З�UP

		ITEM_KIND_MAX,
	};
	
	static ItemObject *Create( const ITEM_KIND &kind, const uint32_t &uniqueID, math::Vector2 pos = DEFAULT_VECTOR2 );
	virtual ~ItemObject(void);

	void	SetPlayerGetFlag();	// �A�C�e���̖�����(����)

	// ���擾
	const TEX_DRAW_INFO &GetDrawInfo() const;
	const uint32_t	&GetUniqueNumber() const{ return m_uniqueNumber; }
	const uint32_t	&GetLiveTime() const{ return m_liveTime; }
	const ITEM_KIND	&GetItemKind() const{ return m_kindItem; }
	const bool		&GetPlayerGetFlag() const{ return m_isPlayerGet; }

protected:

	virtual bool Init() override;						// ������
	virtual bool DieMain() override;					// �h����ł̏�����
	virtual void Update() override;						// �ʒu��AI�ɂ�鐔�l����̍X�V
	//virtual void CollisionUpdate() override{};			// �������l�̍X�V���󂯂Ă̑��N���X�Ƃ̓����蔻�菈��
	virtual void DrawUpdate() override;					// �`��X�V

private:
	
	ItemObject( const ITEM_KIND &kind, const uint32_t &uniqueId, math::Vector2 pos );
	std::string		GetItemFilePath();

	bool			m_isPlayerGet;	// 
	ITEM_KIND		m_kindItem;		// �A�C�e���̎��
	uint32_t		m_uniqueNumber;	// �ق��̃A�C�e���Ƌ�ʂ��邽�߂Ƀ��j�[�N�ԍ�
	uint32_t		m_liveTime;		// ��������Ă���̎���
	Texture2D		m_textureItem;	// �^�C�g���ꖇ�G
};

#endif
