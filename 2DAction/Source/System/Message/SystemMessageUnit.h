/* ====================================================================== */
/**
 * @brief  ���b�Z�[�W�C�x���g������肷����N���X
 *
 * @note
 *		�P�̂Ŏg�p���邱�Ƃ͂Ȃ�
 *		��������ɂ��đ��̃N���X����̃C�x���g���������Ă���
 */
/* ====================================================================== */

#ifndef __MESSAGE_UNIT__
#define __MESSAGE_UNIT__

#include "Common/CommonDefine.h"
#include "System/Message/SystemMessageManager.h"

class SystemMessageUnit
{
public:

	friend SystemMessageManager;
	
	// ����ID�擾
	const uint32_t &GetUniqueId() const{ return m_uniqueUnitId; }

protected:

	SystemMessageUnit(void);
	virtual ~SystemMessageUnit(void);

	virtual void EventUpdate( Common::CMN_EVENT &eventId ){};

private:

	// �ȉ��̊֐��͊�{�I��SystemMessageManager����̂݃A�N�Z�X
	void PushEvent( const Common::CMN_EVENT &eventInfo );
	void StartEventAction();

	uint32_t						m_uniqueUnitId;	// ���ʗpID
	std::vector<Common::CMN_EVENT>	m_eventVec;		// �ق��̃N���X����̃C�x���g
	std::vector<Common::CMN_EVENT>	m_nextEventVec;	// push�҂��̃C�x���g
};

#endif