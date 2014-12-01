#include "SystemMessage.h"

Message::Message(uint32_t kind, void *pt)
: m_msgKind(kind)
, m_ptr(pt)
, m_tellOther(false)
{
}

Message::~Message(void)
{
	//if(ptr){
	//	delete ptr;
	//}
}
