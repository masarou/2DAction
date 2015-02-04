
/* ====================================================================== */
/**
 * @brief  
 *
 * @note 共通の数学系はここに
 *	
 */
/* ====================================================================== */
#ifndef __MATH_COMMON__
#define __MATH_COMMON__

#include "System/SystemDefine.h"

namespace math{

const float PI = 3.1415926535897932384626f;		// π
const float ANGLE_HALF = 180.0f;				// 1周の半分
const float ANGLE_FULL = 360.0f;				// 1周

//!<平方根 
float Sqrtf(float value);
//!<絶対値
int32_t Abs(int32_t value);
float Absf(float value);


//!<最大値 大きいほうを返す
template<class T> T Max(const T& value, const T& max);

//!<最小値 小さいほうを返す
template<class T> T Min(const T& value, const T& min);

//!<クランプ min以上、max以下に納める
template<class T> T Clamp(const T& value, const T& min, const T& max);



} //namespace math

#endif // MATH_COMMON

