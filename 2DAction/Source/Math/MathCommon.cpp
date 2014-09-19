
/* ====================================================================== */
/**
 * @brief  
 *
 * @note 共通の数学系はここに
 *	
 */
/* ====================================================================== */
#include <cmath>
#include "Math/MathCommon.h"

namespace math{


//!<平方根
float Sqrtf(float value){
	return sqrtf(value);
}

//!<絶対値
int32_t Abs(int32_t value){
	return ((value<0)? -value : value);
}
float Absf(float value){
	return ((value<0.0f)? -value : value);
}

//!<最大値 大きいほうを返す
template<class T> T Max(const T& value, const T& max){
	return ( value > max ) ? ( value ) : ( max );
}

//!<最小値 小さいほうを返す
template<class T> T Min(const T& value, const T& min){
	return ( value < min ) ? ( value ) : ( min );
}

//!<クランプ min以上、max以下に納める
template<class T> T Clamp(const T& value, const T& min, const T& max){
	return (( value > max ) ? (max :( value < min ) ? ( min ) : ( value )));
}

} //namespace math

