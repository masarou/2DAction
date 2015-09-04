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

// 二点の距離の累乗を返す
float GetDistance( const Vector2 &vecA, const Vector2 &vecB );

// 二点の距離が指定範囲いないかどうか
bool IsInRange( const Vector2 &vecA, const Vector2 &vecB, const uint32_t &range );
bool IsInRange( const Vector2 &vecA, const Vector2 &vecB, const float &range );

/* ====================================================================== */
/**
 * @brief  グローバル(原点)座標をローカル座標(各オブジェクト毎)に変換
 *
 * @note
 *		angle		:視線と指標となるベクターの成す角(度)
		checkOrigin	:視線
 */
/* ====================================================================== */
Vector2	ChangeCoordinateToRocal( const float &angleToOrigin, const Vector2 &checkOrigin );

}

#endif // MATH_UTILITY