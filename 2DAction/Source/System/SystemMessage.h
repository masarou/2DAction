/* ====================================================================== */
/**
 * @brief  親と子、などのクラス間のやり取りをするメッセージクラス
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
	// m_つける？
	uint32_t msgKind;
	void *ptr;
	bool tellOther;
};

#endif
