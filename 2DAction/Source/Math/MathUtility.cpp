/* ====================================================================== */
/**
 * @brief  
 *
 * @note 角度を扱うクラス
 *		
 */
/* ====================================================================== */

#include "Math/MathCommon.h"
#include "MathUtility.h"

namespace math{

// 二点の距離の累乗を返す
float GetDistance( const Vector2 &vecA, const Vector2 &vecB )
{
	float ret = pow( (vecA.x-vecB.x), 2 ) + pow( (vecA.y-vecB.y), 2 );
	return ret;
}

// 二点の距離が指定範囲いないかどうか
bool IsInRange( const Vector2 &vecA, const Vector2 &vecB, const uint32_t &range )
{
	return IsInRange( vecA, vecB, static_cast<float>(range) );
}
bool IsInRange( const Vector2 &vecA, const Vector2 &vecB, const float &range )
{
	float distance = GetDistance( vecA, vecB );
	return ( distance < (range*range) ? true : false );
}

}