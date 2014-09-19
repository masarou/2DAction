
#ifndef CMN_GAME_UTILITY
#define CMN_GAME_UTILITY

#include "Math/MathCommon.h"
#include "Math/MathVector.h"
#include "Math/MathAngle.h"
#include "System/SystemDefine.h"

class gameUtility{

public:

	static void AddPlayerOffsetPos( math::Vector2 &pos );
	static math::Vector2 GetPlayerOffsetPos();

};

#endif