
#include "picojson.h"


bool picojson::operator==(const value& x, const value& y) {
if (x.is<null>())
    return y.is<null>();
#define PICOJSON_CMP(type)					\
if (x.is<type>())						\
    return y.is<type>() && x.get<type>() == y.get<type>()
PICOJSON_CMP(bool);
PICOJSON_CMP(double);
PICOJSON_CMP(std::string);
PICOJSON_CMP(array);
PICOJSON_CMP(object);
#undef PICOJSON_CMP
assert(0);
#ifdef _MSC_VER
__assume(0);
#endif
return false;
}