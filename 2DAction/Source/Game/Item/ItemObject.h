/* ====================================================================== */
/**
 * @brief  �t�B�[���h�ɖ���ׂɏo�Ă���A�C�e���N���X
 *
 * @note
 *		
 */
/* ====================================================================== */

#ifndef __ITEM_OBJECT__
#define __ITEM_OBJECT__

#include "Game/Game2DBase.h"
#include "System/Task/SystemTaskUnit.h"
#include "System/Collision/SystemCollisionUnit.h"

class ItemObject : public TaskUnit, public Collision2DUnit
{
public:

	// �Œ�l
	static const uint32_t ITEM_LIVE_TIME = 900;	// �A�C�e�����ݎ���
	
	static ItemObject *Create( const Common::ITEM_KIND &kind, math::Vector2 pos = DEFAULT_VECTOR2 );
	virtual ~ItemObject(void);

	// ���擾
	const TEX_DRAW_INFO &GetDrawInfo() const;
	const uint32_t	&GetLiveTime() const{ return m_liveTime; }
	const Common::ITEM_KIND	&GetItemKind() const{ return m_kindItem; }

protected:

	virtual bool Init() override;									// ������
	virtual bool DieMain() override;								// �h����ł̏�����
	virtual void Update() override;									// �ʒu��AI�ɂ�鐔�l����̍X�V
	virtual void DrawUpdate() override;								// �`��X�V
	virtual const Common::TYPE_OBJECT GetTypeObject() const;		// ���������҂Ȃ̂��������^�C�v
	virtual void EventUpdate( Common::CMN_EVENT &eventId );	// �C�x���g�����֐�

private:
	
	ItemObject( const Common::ITEM_KIND &kind, math::Vector2 pos );
	std::string		GetItemFilePath();
 
	Common::ITEM_KIND	m_kindItem;		// �A�C�e���̎��
	uint32_t			m_liveTime;		// ��������Ă���̎���
};

#endif
