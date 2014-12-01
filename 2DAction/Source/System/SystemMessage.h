/* ====================================================================== */
/**
 * @brief  �e�Ǝq�A�Ȃǂ̃N���X�Ԃ̂��������郁�b�Z�[�W�N���X
 *
 * @note
 */
/* ====================================================================== */

#ifndef SYSTEM_MESSAGE
#define SYSTEM_MESSAGE

#include "SystemDefine.h"

class Message
{
public:
	Message(uint32_t kind, void *pt = NULL);
	Message(void);
	~Message(void);

	const uint32_t getSignal(){
		return m_msgKind;
	}

	void* getPtr(){
		return m_ptr;
	}

private:

	uint32_t	m_msgKind;
	void		*m_ptr;
	bool		m_tellOther;
};

#endif
