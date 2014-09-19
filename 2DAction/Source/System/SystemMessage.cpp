#include "SystemMessage.h"

Message::Message(uint32_t kind, void *pt)
: msgKind(kind)
, ptr(pt)
, tellOther(false)
{
}

Message::~Message(void)
{
	//if(ptr){
	//	delete ptr;
	//}
}
