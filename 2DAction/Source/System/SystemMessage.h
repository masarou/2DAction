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
		return msgKind;
	}

	void* getPtr(){
		return ptr;
	}

private:
	// m_����H
	uint32_t msgKind;
	void *ptr;
	bool tellOther;
};

#endif
