/* ====================================================================== */
/**
 * @brief  
 *
 * @note 数学的なUtility
 *		
 */
/* ====================================================================== */
#ifndef __MATH_UTILITY__
#define __MATH_UTILITY__

#include "System/SystemDefine.h"
#include "Math/MathVector.h"

namespace math{

// 内積を求める
float GetInnerProduct( math::Vector2 vecA, math::Vector2 vecB );

// 二点の距離の累乗を返す
float GetDistance( const Vector2 &vecA, const Vector2 &vecB );

// 二点の距離が指定範囲いないかどうか
bool IsInRange( const Vector2 &vecA, const Vector2 &vecB, const uint32_t &range );
bool IsInRange( const Vector2 &vecA, const Vector2 &vecB, const float &range );

// 成す角を求める
float GetAngleFormed( const math::Vector2 &vecA, const math::Vector2 &vecB);

}

#endif // MATH_UTILITY