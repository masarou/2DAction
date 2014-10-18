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

class ItemObject
{
public:

	// �Œ�l
	static const uint32_t ITEM_LIVE_TIME = 900;	// �A�C�e�����ݎ���

	enum ITEM_KIND{
		ITEM_RAPID_BULLET,	// �A�ˑ��xUP
		ITEM_LIFE_UP,		// ���C�t��
		ITEM_DAMAGE_UP,		// �e�̈З�UP

		ITEM_MAX,
	};

public:

	ItemObject( const ITEM_KIND &kind, const uint32_t &uniqueId, const math::Vector2 &pos );
	virtual ~ItemObject(void);
	
	void	Update();
	void	Draw();

	// ���擾
	const TEX_DRAW_INFO &GetDrawInfo();
	uint32_t GetUniqueNumber(){ return m_uniqueNumber; }
	uint32_t GetLiveTime(){ return m_liveTime; }

private:

	std::string		GetItemFilePath();

	ITEM_KIND		m_kindItem;		// �A�C�e���̎��
	uint32_t		m_uniqueNumber;	// �ق��̃A�C�e���Ƌ�ʂ��邽�߂Ƀ��j�[�N�ԍ�
	uint32_t		m_liveTime;		// ��������Ă���̎���
	Game2DBase		*m_drawItem;	// �A�C�e���`��N���X
	TEX_DRAW_INFO	m_itemInfo;		// �A�C�e���`����

};

#endif
