/* ====================================================================== */
/**
 * @brief  
 *
 * @note 数学的なUtility
 *		
 */
/* ====================================================================== */
#ifndef MATH_UTILITY
#define MATH_UTILITY

#include "System/SystemDefine.h"
#include "Math/MathVector.h"

namespace math{

// 二点の距離の累乗を返す
float GetDistance( const Vector2 &vecA, const Vector2 &vecB );


// 二点の距離が指定範囲いないかどうか
bool IsInRange( const Vector2 &vecA, const Vector2 &vecB, const uint32_t &range );
bool IsInRange( const Vector2 &vecA, const Vector2 &vecB, const float &range );


}

#endif // MATH_UTILITY