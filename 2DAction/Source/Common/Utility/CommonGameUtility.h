
#ifndef CMN_GAME_UTILITY
#define CMN_GAME_UTILITY

#include "Math/MathCommon.h"
#include "Math/MathVector.h"
#include "Math/MathAngle.h"
#include "Math/MathUtility.h"
#include "System/SystemDefine.h"
#include "System/Draw2D/SystemDraw2DDefine.h"


void AddPlayerOffsetPos( math::Vector2 &pos );

const math::Vector2 GetPlayerOffsetPos();

const bool IsInRangeTexture( const TEX_DRAW_INFO &texA, const TEX_DRAW_INFO &texB );

#endif