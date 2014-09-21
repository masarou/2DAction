/* ====================================================================== */
/**
 * @brief  
 *
 * @note 角度を扱うクラス
 *		
 */
/* ====================================================================== */

#include "MathUtility.h"

namespace math{

// 二点の距離の累乗を返す
uint32_t GetDistance( const Vector2 &vecA, const Vector2 &vecB )
{
	uint32_t ret = vecA.x*vecA.x + vecB.y*vecB.y;
	return ret;
}

// 二点の距離が指定範囲いないかどうか
bool IsInRange( const Vector2 &vecA, const Vector2 &vecB, const uint32_t &range )
{
	return IsInRange( vecA, vecB, static_cast<float>(range) );
}
bool IsInRange( const Vector2 &vecA, const Vector2 &vecB, const float &range )
{
	uint32_t distance = GetDistance( vecA, vecB );
	return ( static_cast<float>(distance) < (range*range) ? true : false );
}

}